require 'sinatra'

set :logging, true

@@status = 'red'
COLORS = %w(red green blue yellow)
DEFAULT_COLOR = COLORS.first

get '/' do
 logger.info "sending status: #{get_status}"
 get_status
end
  
get %r{/(red|green|blue|yellow)} do |color|
  set_status(color)
end
  
def set_status(status)
  @@status = status
end

def get_status
  "<color>#{COLORS.include?(@@status) ? @@status : DEFAULT_COLOR}</color>"
end

