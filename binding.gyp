#
# Copyright (c) 2015 Axel Isouard <ax@eml.cc>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

{
  'includes': [
    'third_party/webrtc/chromium/src/build/common.gypi',
    'third_party/webrtc/webrtc/build/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'nwebrtc',
      'dependencies': [
        'third_party/webrtc/talk/libjingle.gyp:libjingle_peerconnection'
      ],
      'include_dirs': [
        'third_party/webrtc'
      ],
      'sources': [
        'src/binding.cc',
        'src/RTCDataChannel.cc',
        'src/RTCIceCandidate.cc',
        'src/RTCPeerConnection.cc',
        'src/RTCSessionDescription.cc'
      ],
      'cflags': [
        '-Wno-deprecated-declarations',
      ],
      'ldflags!': [
        '-Wl,-z,defs'
      ]
    },
    {
      'target_name': 'action_after_build',
      'type': 'none',
      'dependencies': [ '<(module_name)' ],
      'copies': [
        {
          'files': [ '<(PRODUCT_DIR)/<(module_name).node' ],
          'destination': '<(module_path)'
        }
      ]
    }
  ]
}
