#! /usr/bin/lua

-- Warning! this script should be executed on root to lookup vendor md5.
package.path = './vendor/lualibs/md5/?.lua;' .. package.path -- our vendor scripts

--
-- Based on awk version from valgrind docs: https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
-- This script does the same as its awk counterpart, however its written in lua :D
--
--  1. Navigate through lines, if a "{" is found then mark that we are inside a suppresion rule.
--  2. Update hash and store following lines if we are inside a suppresion rule.
--  3. When a "}" is found, then mark we are outside a suppression rule, replace the placeholder
--    to an actual name (hash) and print the carried lines if they are not already printed (also checked by hash).
--
local md5 = require('md5')
local insidesup = false
local alreadyPrint = {}
local currentSum = nil
local supplines = ""

function shouldPrint(desired)
    for ix, hash in ipairs(alreadyPrint) do
        if desired == hash then
            return false
        end
    end

    return true
end

while true do
    local line = io.read()
    if line == nil then break end

    if string.match(line, "{") then
        insidesup = true
        currentSum = md5.new()
    end

    if insidesup then
        supplines = supplines .. line .. "\n"
        currentSum:update(line)
    end

    if string.match(line, "}") and insidesup then
        insidesup = false
        hash = md5.tohex(currentSum:finish())
        if shouldPrint(hash) then
            table.insert(alreadyPrint, hash)
            supplines = string.gsub(supplines, '<insert_a_suppression_name_here>', hash)
            print(supplines)
        end
    end
end
