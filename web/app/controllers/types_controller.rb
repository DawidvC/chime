class TypesController < ApplicationController
  def index
    redirect_to :action => 'value_type'
  end
end
