## Howto use git command line

- Download it: https://git-scm.com/downloads
- Online help: https://try.github.io

| command    | description          |
|------------|----------------------|
| git init   | initialize in current dir in **/.git/** |
| git status | show untracked files |

### download all
| command    | description          |
|------------|----------------------|
| git clone git://github.com/manel3/usbpower.git ||

### upload
| command    | description          |
|------------|----------------------|
| git add File File2             | add files to the staging area to start tracking changes in them |
| git add '*'                    | add a bunch of files |
| git commit -m "Commit message" | store staged changes with a message describing what changed |
| git log                        | browse pending commits |
| git remote add origin https://github.com/manel3/usbpower.git | add a remote repository to push our local repo |
| git push -u origin master                                    | -u remember values to use only **git push** |
| git push    ||

### download
| command    | description          |
|------------|----------------------|
| git pull origin master | check for changes on our GitHub repository |
| git diff HEAD          | take a look at what is **different**, diff to most recent commit using HEAD pointer  |
| git diff --staged      | see the changes you just staged |

### remove
| command    | description          |
|------------|----------------------|
| git reset dir/file.txt | unstage files |

### undo
| command    | description          |
|------------|----------------------|
| git checkout -- file | change back to the last commit |

### branching
| command    | description          |
|------------|----------------------|
| git branch clean_up              | create a branch called clean_up |
| git branch                       | show branches |
| git checkout clean_up            | Switching Branches |
| git rm '*.txt'                   | remove actual files from disk, and stage removal of files |
| git commit -m "Remove all files" | commit changes |

### Preparing to Merge
| command    | description          |
|------------|----------------------|
| git checkout master    | Switching Back to master branch |
| git merge clean_up     | merge changes from the clean_up branch into the master branch |
| git branch -d clean_up | delete branch |
| git push               ||
