#!/usr/bin/env ruby

def prepend_line(original_file, line, replace)
  new_file = original_file + '.new'
  text = File.read(original_file)
  text.sub!(replace, '')
  
  File.open(new_file, 'w') do |fo|
    fo.puts line
    fo.write text.lstrip
  end

  File.rename(original_file, original_file + '.old')
  File.rename(new_file, original_file)
end

replace_regex = /\/\*\s-\*-\s.*\s-\*-\s\*\//m
text_to_add = '/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */'

ARGV.each do |f|
  unless text_to_add == File::open(f, &:readline).chomp
    prepend_line(f, text_to_add, replace_regex)
  end
end
