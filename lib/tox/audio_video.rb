# frozen_string_literal: true

module Tox
  ##
  # Tox audio/video instance.
  #
  class AudioVideo
    def initialize(client)
      @on_call = nil
      @on_call_state_change = nil
      @on_audio_frame = nil
      @on_video_frame = nil

      initialize_with client
    end

    def on_call(&block)
      @on_call = block
    end

    def on_call_state_change(&block)
      @on_call_state_change = block
    end

    def on_audio_frame(&block)
      @on_audio_frame = block
    end

    def on_video_frame(&block)
      @on_video_frame = block
    end
  end
end
