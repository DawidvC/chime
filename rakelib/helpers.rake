# Rakefile: helpers

class String
  def red
    "\e[31m#{self}\e[0m"
  end
  def bold
    "\e[1m#{self}\e[0m"
  end
end

def log(key, message)
  puts("[#{key}]".bold + ": " + message)
end