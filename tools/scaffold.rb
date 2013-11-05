#!/usr/bin/env ruby

# Creates .h and .cpp files for C++ class in working directory.
def touch_cpp_class(klass)
  h_file = klass + '.h'
  cpp_file = klass + '.cpp'

  raise "Would overwrite #{h_file}!" if File::exists?(h_file)
  raise "Would overwrite #{cpp_file}!" if File::exists?(cpp_file)

  File.open(h_file, 'w') do |f|
    guard = h_file.gsub('.', '_').upcase
    f.puts <<-EOS.gsub(/^ {6}/, '')
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
    f.puts <<-EOS.gsub(/^ {6}/, '')
      /* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
      #include "#{h_file}"

      #{klass}::#{klass}()
      {
        // ctor code
      }
    EOS
  end
end


# Creates test file in working directory
def touch_boost_test(test_name)
  test_file = test_name.downcase + '_test.cpp'

  raise "Would overwrite #{test_file}" if File::exists?(test_file)

  File.open(test_file, 'w') do |f|
    f.puts <<-EOS.gsub(/^ {6}/, '')
      /* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
      #define BOOST_TEST_DYN_LINK
      #define BOOST_TEST_MODULE #{test_name}Test
      #include <boost/test/unit_test.hpp>

      BOOST_AUTO_TEST_CASE(#{test_name.downcase}_test)
      {
          //
      }
    EOS
  end
end

# main

usage_text = "Usage: #{$PROGRAM_NAME} <class|test> name"

if ARGV.count == 2
  case ARGV[0]
    when "class"
      touch_cpp_class(ARGV[1])
    when "test"
      touch_boost_test(ARGV[1])
    else
      puts "Unknown scaffold!"
      puts usage_text
  end
else
  puts usage_text
end
