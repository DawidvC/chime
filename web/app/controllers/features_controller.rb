class FeaturesController < ApplicationController
  def index
    redirect_to :action => 'flow_control'
  end
end
