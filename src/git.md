# git(1)

## Misc
```markdown
  git add -p [<file>] ............ partial staging (interactive)
```

## remote
```markdown
  git remote -v .................. list remotes verbose (with URLs)
  git remote show [-n] <remote> .. list info for <remote> (like remote HEAD,
                                   remote branches, tracking mapping)
```

## branching
```markdown
  git branch [-a] ................ list available branches; -a to include
                                   remote branches
  git branch -vv ................. list branch & annotate with head sha1 &
                                   remote tracking branch
  git branch <bname> ............. create branch with name <bname>
  git checkout <bname> ........... switch to branch with name <bname>
  git push -u origin <rbname> .... push branch to origin (or other remote), and
                                   setup <rbname> as tracking branch
```

## resetting
```markdown
  git reset [opt] <ref|commit>
    opt:
      --mixed .................... resets index, but not working tree
      --hard ..................... matches the working tree and index to that
                                   of the tree being switched to any changes to
                                   tracked files in the working tree since
                                   <commit> are lost
  git reset HEAD <file> .......... remove file from staging
  git reset --soft HEAD~1 ........ delete most recent commit but keep work
  git reset --hard HEAD~1 ........ delete most recent commit and delete work
```

## tags
```markdown
  git tag -a <tname> -m "descr" ........ creates an annotated tag (full object
                                         containing tagger, date, ...)
  git tag -l ........................... list available tags
  git checkout tag/<tname> ............. checkout specific tag
  git checkout tag/<tname> -b <bname> .. checkout specific tag in a new branch
```

## diff
```markdown
  git diff HEAD:<fname> origin/HEAD:<fname> ... diff files for different refs
  git diff -U$(wc -l <fname>) <fname> ......... shows complete file with diffs
                                                instead of usual diff snippets
```

## log
```markdown
  git log --oneline .... shows log in single line per commit -> alias for
                         '--pretty=oneline --abbrev-commit'
  git log --graph ...... text based graph of commit history
  git log --decorate ... decorate log with REFs
```

## patching
```markdown
  git format-patch <opt> <since>/<revision range>
    opt:
      -N ................... use [PATCH] instead [PATCH n/m] in subject when
                             generating patch description (for patches spanning
                             multiple commits)
      --start-number <n> ... start output file generation with <n> as start
                             number instead '1'
    since spcifier:
      -3 .................. e.g: create a patch from last three commits
      <comit hash> ........ create patch with commits starting after <comit hash>

  git am <patch> ......... apply patch and create a commit for it

  git apply --stat <PATCH> ... see which files the patch would change
  git apply --check <PATCH> .. see if the patch can be applied cleanly
  git apply <PATCH> .......... apply the patch locally without creating a commit

  # eg: generate patches for each commit from initial commit on
  git format-patch -N $(git rev-list --max-parents=0 HEAD)

  # generate single patch file from a certain commit/ref
  git format-patch <COMMIT/REF> --stdout > my-patch.patch
```

## submodules
```markdown
  git submodule add <url> [<path>] .......... add new submodule to current project
  git clone --recursive <url> ............... clone project and recursively all
                                              submodules (same as using
                                              'git submodule update --init
                                              --recursive' after clone)
  git submodule update --init --recursive ... checkout submodules recursively
                                              using the commit listed in the
                                              super-project (in detached HEAD)
  git submodule update --remote <submod> .... fetch & merge remote changes for
                                              <submod>, this will pull
                                              origin/HEAD or a branch specified
                                              for the submodule
```

## inspection
```markdown
  git ls-tree [-r] <ref> .... show git tree for <ref>, -r to recursively ls sub-trees
  git show <obj> ............ show <obj>
  git cat-file -p <obj> ..... print content of <obj>
```

## revision_range
```markdown
  HEAD ........ last commit
  HEAD~1 ...... last commit-1
  HEAD~N ...... last commit-N (linear backwards when in tree structure, check
                difference between HEAD^ and HEAD~)
  git rev-list --max-parents=0 HEAD ........... first commit
```