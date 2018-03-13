{
  "targets": [
    {
      "target_name": "binding",
      "defines": [
        "HUNSPELL_STATIC"
      ],
      "sources": [
        "binding.cc"
      ],
      "dependencies": [
        "hunspell"
      ]
    },
    {
      "target_name": "hunspell",
      "type": "static_library",
      "defines": [
        "HUNSPELL_STATIC"
      ],
      "sources": [
        "node_modules/hunspell/src/hunspell/affentry.cxx",
        "node_modules/hunspell/src/hunspell/affixmgr.cxx",
        "node_modules/hunspell/src/hunspell/affixmgr.hxx",
        "node_modules/hunspell/src/hunspell/csutil.cxx",
        "node_modules/hunspell/src/hunspell/filemgr.cxx",
        "node_modules/hunspell/src/hunspell/hashmgr.cxx",
        "node_modules/hunspell/src/hunspell/hunspell.cxx",
        "node_modules/hunspell/src/hunspell/hunzip.cxx",
        "node_modules/hunspell/src/hunspell/phonet.cxx",
        "node_modules/hunspell/src/hunspell/replist.cxx",
        "node_modules/hunspell/src/hunspell/suggestmgr.cxx"
      ]
    }
  ]
}