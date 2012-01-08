require "test/unit"

class TestLibraryFileName < Test::Unit::TestCase
  def test_module_level_captures_variable_used
    output = `#{TEST_SCRIPTS_PATH}/ModuleLevelCapturedVariableUsed 2>&1`
    
    lines = output.split("\n")
    
    assert_equal("0",       lines[0])
    assert_equal("1",       lines[1])
    assert_equal("2",       lines[2])
    assert_equal("3",       lines[3])
    assert_equal("4",       lines[4])
    assert_equal("success", lines[5])
  end
end