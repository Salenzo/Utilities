#!/usr/bin/env ruby

lines = [
  "#include <stdio.h>",
  "#include <stdlib.h>",
  "",
  "void CPRO_ATEXIT(void){FILE*f=fopen(\"~~cpro.c\",\"wb\");",
  "",
  "fclose(f);}",
]
i = 0
code = ARGV[0] ? File.read(ARGV.shift) : $stdin.read
code.each_line do |line|
  line.chomp!
  line.gsub!(/^\s+|\s+$/, "")
  flag = false
  if line =~ /\{$/
    if line =~ /^(if|\}?\s*else|for|while|do)/
      flag = true
    else
      tmp = code.split("\n")
      tmp[i] << "CPRO_TEST=0;"
      tmp = "volatile char CPRO_TEST;\n" + tmp.join("\n")
      File.write("~~cpro.c", tmp)
      print "Probing line #{i + 1}..."
      if system("gcc", "~~cpro.c", "-fsyntax-only", "-w", *ARGV, :out => "nul", :err => "nul")
        flag = true
        puts "as counted."
      else
        puts "is inappropriate."
      end
    end
  end
  if flag
    line << "CPRO_TRACE#{i}++;"
    lines[2] << "static unsigned CPRO_TRACE#{i};"
    lines[4] << "fprintf(f,\"#{i},%d\\n\",CPRO_TRACE#{i});"
    line << "atexit(CPRO_ATEXIT);" if line =~ /^int\s+main\(/
  end
  lines << line
  i += 1
end
File.write("~~cpro.c", lines.join("\n"))
at_exit { File.delete("~~cpro.c") }
puts "Compiling to a.exe."
system "gcc", "~~cpro.c", *ARGV
exit $? if $? != 0
puts "Running a.exe."
system "a.exe", :out => "nul", :err => :out
lines = code.each_line.to_a.map { |line| ("        " + line.chomp).rstrip }
File.read("~~cpro.c").each_line do |line|
  i, count = line.split(",").map(&:to_i)
  lines[i].sub!(/^.{8}/, "%7d " % count) if i and count
end
filename = "profile-#{Time.now.utc.strftime("%Y-%m-%d-%H-%M-%S")}.c"
File.write(filename, lines.join("\n"))
puts "Profiler report saved to #{filename}."
