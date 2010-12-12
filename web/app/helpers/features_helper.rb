module FeaturesHelper
  def feature_section(section_name, &block)
    html = render("features_navigation")
    
    html += "<section><h2>#{section_name}</h2><code>\r"
    
    html += capture(&block)
    
    html += "</code></section>\r"
    
    html
  end
end
