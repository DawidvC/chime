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
  length = 7 - key.length
  length = key.length if length < 0
  
  filler = ' ' * length
  
  puts(filler + "[#{key}]".bold + ": " + message)
end
