# Rakefile: clean

PCH_FILES = FileList['**/*.gch']

namespace(:pch) do
  
  desc("Remove all pch files")
  task(:remove) do
    sh("rm -rf #{PCH_FILES}")
  end
end
