# frozen_string_literal: true

# Abstract status classes.
require 'tox/status/base'
require 'tox/status/json_api'

# Status classes for direct usage.
require 'tox/status/official'

module Tox
  ##
  # Tox network status from multiple sources.
  #
  module Status
    def self.official
      Official.new
    end
  end
end
