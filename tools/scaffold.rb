#!/usr/bin/env ruby

def touch_cpp_class(klass)
  h_file = klass + '.h'
  cpp_file = klass + '.cpp'

  raise "Would overwrite #{h_file}!" if File::exists?(h_file)
  raise "Would overwrite #{cpp_file}!" if File::exists?(cpp_file)

  File.open(h_file, 'w') do |f|
    guard = h_file.gsub('.', '_').upcase
    f.puts <<EOS
/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef #{guard}
#define #{guard}

class #{klass}
{
 public:
    #{klass}();

 private:

};

#endif // #{guard}
EOS
  end

  File.open(cpp_file, 'w') do |f|
    f.puts <<EOS
/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#{klass}::#{klass}()
{
  // ctor code
}
EOS
  end
end

# main

if ARGV[0]
  touch_cpp_class(ARGV[0])
else
  puts "Usage: #{$PROGRAM_NAME} class_name"
end
