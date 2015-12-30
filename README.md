# Qiktionary
An English-Vietnamese dictionary project based on 2 basic data structures: AVL tree & Hash table.

### Directory Layout
```
.
├── /avlCore/
│   ├── avlDict.cpp
│   ├── avlDict.h
│   └── BST.h
├── /data/
│   ├── /readme_files/
│   ├── icon.ico
│   ├── README.html
│   └── tu_dien.dat
├── /hashCore/
│   ├── dict.h
│   ├── extList.h
│   ├── hashDict.cpp
│   └── hashDict.h
├── /mainApp/
│   ├── button.cpp
│   ├── button.h
│   ├── caption.cpp
│   ├── caption.h
│   ├── dest.cpp
│   ├── dest.h
│   ├── main.cpp
│   ├── status.cpp
│   ├── status.h
│   └── vars.h
├── Qiktionary.sln
├── Qiktionary.v12.suo
└── Qiktionary.vcxproj
```

### Features
+ Friendly User Interface
+ Look-up words
+ Translate sentences
+ Multi-thread (for loading library by 2 methods at the same time)
+ Distributed by 1 fully setup package

### Risks
+ Loading library into AVL structure may take up-to 5 minutes or more (cause its particular properties)
+ Uses much RAM (up-to 500MB)
+ Some glitches in UI

### Technology
+ AVL tree (OOP)
+ Hash table (with Bernstein's hash function)
  http://www.strchr.com/hash_functions
+ Microsoft win32 API
+ Multi-threading

### Usages
+ For _help_, press "?" button on caption bar

  ![alt text](https://raw.githubusercontent.com/phamtri2395/Qiktionary/master/readme_files/help_button.png "Help button")

+ Write in upper textbox & press **ENTER** to look-up for words (translate for sentences), result will appears in lower textbox

  ![alt-text](https://raw.githubusercontent.com/phamtri2395/Qiktionary/master/readme_files/translate.png "Translate")

+ Choose data structure by clicking on their name

  ![alt-text](https://raw.githubusercontent.com/phamtri2395/Qiktionary/master/readme_files/algorithm.png "Change algorithm")

+ _Status_ bar warns you about dictionary loading progress:

  ![alt-text](https://raw.githubusercontent.com/phamtri2395/Qiktionary/master/readme_files/status.png "Status bar")

  _"READY"_:   means all words was loaded into memory & ready for usage
  
  _"Loading"_: means just some words was loaded & the others will **not available**
+ Which word can't be found in dictionary will be shown as _"null"_

  ![alt-text](https://raw.githubusercontent.com/phamtri2395/Qiktionary/master/readme_files/null.png "If can't find word")

### Credits
A thank to our teacher Ho Thi Thanh Tuyen, who always pushs our limit and teachs us devotedly.

### License

### Support
Email: phamtri2395@gmail.com
