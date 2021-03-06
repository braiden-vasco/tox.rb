# frozen_string_literal: true

module Tox
  ##
  # Binary primitive representation.
  #
  class Binary
    using CoreExt

    abstract_class_method :bytesize

    def self.hex_re
      /\A[\da-fA-F]{#{2 * bytesize}}\z/
    end

    attr_reader :value

    inspect_string { to_s.inspect }

    def initialize(value)
      String.ancestor_of! value

      if value.bytesize == self.class.bytesize
        self.value = value
      elsif value =~ self.class.hex_re
        self.hex_value = value
      else
        raise ArgumentError, 'expected value to be a hex or binary string'
      end
    end

    def to_s
      @to_s ||= value.unpack('H*').first.upcase.freeze
    end

    def ==(other)
      return false unless self.class == other.class
      value == other.value
    end

  private

    def value=(value)
      @value = value.dup_and_freeze
    end

    def hex_value=(value)
      @value = [value].pack('H*').freeze
    end
  end
end
