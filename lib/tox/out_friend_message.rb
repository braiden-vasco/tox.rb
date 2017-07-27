# frozen_string_literal: true

# tox.rb - Ruby interface for libtoxcore
# Copyright (C) 2015-2017  Braiden Vasco
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

module Tox
  ##
  # Outgoing friend message representation in Tox client.
  #
  class OutFriendMessage
    attr_reader :friend, :id

    def initialize(friend, id)
      self.friend = friend
      self.id = id
    end

    def client
      friend.client
    end

  private

    def friend=(value)
      raise TypeError, "expected friend to be a #{Friend}" unless value.is_a? Friend
      @friend = value
    end

    def id=(value)
      raise TypeError, "expected id to be an #{Integer}"                     unless value.is_a? Integer
      raise ArgumentError, 'expected id to be greater than or equal to zero' unless value >= 0
      @id = value
    end
  end
end
