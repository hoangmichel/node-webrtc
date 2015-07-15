{
  'includes': [
    'third_party/webrtc/chromium/src/build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'peeracle-webrtc',
      'dependencies': [
        'third_party/webrtc/talk/libjingle.gyp:libjingle_peerconnection',
      ],
      'include_dirs': [
        'third_party/webrtc',
      ],
      'sources': [
        'src/binding.cc',
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
