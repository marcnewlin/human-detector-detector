#!/usr/bin/env python3

import binascii
import serial
import struct
import time
import os
import sys
from threading import Thread, Lock

GPIOA = 0x00
GPIOB = 0x01

GPIO_INPUT  = 0x00
GPIO_OUTPUT = 0x10
GPIO_MUX    = 0x08
GPIO_ANALOG = 0x03

GPIO_PULL_NONE = 0x04
GPIO_PULL_UP   = 0x18
GPIO_PULL_DOWN = 0x28

CFG_GPIO_PIN = 0x1000
CFG_DMA = 0x1001
CFG_ADC = 0x1002
TRIGGER_ADC = 0x1003
READ_ADC = 0x1004
SET_GPIO_PIN = 0x1005


class Command:

  def __init__(self, cmd_code, args=[]):
    self.cmd_code = cmd_code
    self.args = [a for a in args if a is not None]

  def serialize(self):
    return struct.pack("I%s"%("I"*len(self.args),), self.cmd_code, *self.args)


class Client:

  def __init__(self):
    self.tty = serial.Serial("/dev/ttyACM0", timeout=0.2)

  def write(self, data):
    self.tty.write(data)

  def read(self, count=8):
    return self.tty.read(count)

  def read2(self, count, timeout):
    data = b""
    start = time.time()
    while (time.time()-start) < timeout and len(data) < count:
      data += self.read(count)
    return data

  def configure_dma(self):
    cmd = Command(CFG_DMA, [])
    self.write(cmd.serialize())
    cmd_code, status = struct.unpack("II", self.read())
    if cmd_code != CFG_DMA or status != 0:
      sys.stderr.write("error! configure_dma failed!")
      sys.stderr.write(cmd_code, status)

  def configure_adc(self):
    cmd = Command(CFG_ADC, [])
    self.write(cmd.serialize())
    d = self.read()
    cmd_code, status = struct.unpack("II", d)
    if cmd_code != CFG_ADC or status != 0:
      sys.stderr.write("error! configure_adc failed!")
      sys.stderr.write(cmd_code, status)

  def trigger_adc(self):
    cmd = Command(TRIGGER_ADC, [])
    self.write(cmd.serialize())
    d = self.read()
    cmd_code, status = struct.unpack("II", d)
    if cmd_code != TRIGGER_ADC or status != 0:
      sys.stderr.write("error! trigger_adc failed!")
      sys.stderr.write(cmd_code, status)

  def read_adc(self):
    with open("output.iq", "wb") as f:
      cmd = Command(READ_ADC, [])
      self.write(cmd.serialize())
      while True:
        data = self.read2(3072, 2)
        yield(data)
        f.write(data)
        f.flush()


  def configure_gpio(self, group, pin, mode, value=None):
    cmd = Command(CFG_GPIO_PIN, [group, pin, mode, value])
    pld = cmd.serialize()
    self.write(pld)
    response = self.read()
    print(response)
    cmd_code, status = struct.unpack("II", response)
    if cmd_code != CFG_GPIO_PIN or status != 0:
      print("error! configure_gpio failed!", file=sys.stderr)
      print(cmd_code, status, file=sys.stderr)

c = Client()

# ADC Inputs
c.configure_gpio(GPIOA, 6, GPIO_ANALOG)
c.configure_gpio(GPIOA, 7, GPIO_ANALOG)
c.configure_gpio(GPIOB, 0, GPIO_ANALOG)
c.configure_gpio(GPIOB, 1, GPIO_ANALOG)

# c.configure_gpio(GPIOB, 2, GPIO_OUTPUT, 0)
# c.configure_gpio(GPIOA, 9, GPIO_OUTPUT, 1)
# c.configure_gpio(GPIOA, 10, GPIO_OUTPUT, 1)

c.configure_dma()
c.configure_adc()
c.trigger_adc()

start = time.time()
sample_count = 0
for data in c.read_adc():

  # unpack IQ from SC12 -> SC16
  shorts_out = []
  while len(data) >= 3:
    I = (data[0]<<4) | (data[1]>>4)
    Q = ((data[1]&0xf)<<8) | data[2]
    data = data[3:]
    shorts_out.append(I)
    shorts_out.append(Q)

  # complex shorts -> stdout (to eg. baudline)
  sample_count += len(shorts_out)/2
  data_out = struct.pack("H"*len(shorts_out), *shorts_out)
  sys.stdout.buffer.write(data_out)
  sys.stdout.flush()

  # print the sample rate once per second
  if (time.time()-start) >= 1.0:
    sys.stderr.buffer.write(b"%d samples per second\n" % (sample_count/(time.time()-start)))
    sys.stderr.flush()
    sample_count = 0
    start = time.time()
