class TypesController < ApplicationController
  def index
    redirect_to :action => 'object'
  end
end
