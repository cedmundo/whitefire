#! /usr/bin/lua5.3

-- Converts from a YAML file to a DESC file.
-- Usage:
--  cat in.yaml | ./desc-encoder.lua > out.desc
--
local lyaml  = require('lyaml')
local msgpack = require('MessagePack')
local data = lyaml.load(io.read('*all'))
msgpack.set_number('double')
msgpack.set_string('string')
print(msgpack.pack(data))
