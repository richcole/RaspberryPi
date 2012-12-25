result = File.open(ARGV[0]).read.gsub("\r", "")
File.open(ARGV[0], "w") do |file| file.write(result) end
