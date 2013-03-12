#!/usr/bin/env ruby

def prepend_line(original_file, line)
  new_file = original_file + '.new'

  File.open(new_file, 'w') do |fo|
    fo.puts line
    File.foreach(original_file) do |li|
      fo.puts li
    end
  end

  File.rename(original_file, original_file + '.old')
  File.rename(new_file, original_file)
end

text_to_add = '/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */'

ARGV.each do |f|
  unless text_to_add == File::open(f, &:readline).chomp
    prepend_line(f, text_to_add)
  end
end
