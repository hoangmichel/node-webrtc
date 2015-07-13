{
  'includes': [
    'webrtc/webrtc/build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'peeracle-webrtc',
      'dependencies': [
        'webrtc/talk/libjingle.gyp:libjingle_peerconnection',
      ],
      'include_dirs': [
        'webrtc',
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
