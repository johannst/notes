# git(1)

## Working areas
```text
+-------------------+ --- stash -----> +-------+
| working directory |                  | stash |  // Shelving area.
+-------------------+ <-- stash pop -- +-------+
      |       ^
     add      |
      |     reset
      v       |
+-------------------+
|   staging area    |
+-------------------+
      |
    commit
      |
      v
+-------------------+
| local repository  |
+-------------------+
      |       ^
     push     |
      |     fetch /
      |      pull
      v       |
+-------------------+
| remote repository |
+-------------------+
```

## Clean
```markdown
  git clean -X ......... remove only ignored files (-n for dry run)
  git clean -f -d -x ... remove untracked & ignored files / folders
  git clean -e <pat> ... exclude pattern from deletion
```

## Staging
```markdown
  git add -p [<file>] ............ partial staging (interactive)
```

## Remote
```markdown
  git remote -v .................. list remotes verbose (with URLs)
  git remote show [-n] <remote> .. list info for <remote> (like remote HEAD,
                                   remote branches, tracking mapping)
```

## Branching
```markdown
  git branch [-a] ................ list available branches; -a to include
                                   remote branches
  git branch -vv ................. list branch & annotate with head sha1 &
                                   remote tracking branch
  git branch <bname> ............. create local branch with name <bname>
  git branch -d <bname> .......... delete local branch with name <bname>
  git checkout <bname> ........... switch to branch with name <bname>
  git checkout --track <branch> .. start to locally track a remote branch
  git branch --unset-upstream .... unset remote tracking branch

  # Remote

  git push -u origin <rbname> ........ push local branch to origin (or other
                                       remote), and setup <rbname> as tracking
                                       branch
  git push origin --delete <rbname> .. delete branch <rbname> from origin (or
                                       other remote)
```

## Update local from remote
```markdown
  git fetch --prune .................. update all remote references and
                                       remove delete non-existing ones
                                       (does not merge into local tracking branch)
  git pull [--rebase] ................ fetch remote references and merge into
                                       local tracking branch (fast-forward by default).
                                       Optionally rebase local tracking branch
                                       on-top of remote branch (in case local
                                       branch has additional commits compared to remote branch).
```

## Tags
```markdown
  git tag -a <tname> -m "descr" ........ creates an annotated tag (full object
                                         containing tagger, date, ...)
  git tag -l ........................... list available tags
  git checkout tag/<tname> ............. checkout specific tag
  git checkout tag/<tname> -b <bname> .. checkout specific tag in a new branch

  # Remote

  git push origin --tags .... push local tags to origin (or other remote)
```

## Merging
```markdown
  git merge [opt] <commit> .... integrate changes from <commit> since
    opt:                        current branch and <commit> diverged
      --squash ................ merge all commits into a single one
      --no-commit ............. dont generate commit if the merge succeeds

  git merge-base <commit> <commit>
                                get the common ancestor, since both commits diverged

  git rebase -i <upstream> .... interactively rebase on <upstream>, also supports actions
                                like squashing, editing, rewording, etc of commits

  git cherry-pick <commit> .... apply commit on current branch
```

## Worktree
Worktrees allow to maintain multiple working trees in the filesystem linked to
the same repository (shared .git folder).
```markdown
  git worktree add <path> .............. create a tree at <path> with a new branch
                                         checked out (bname is basename of <path>)
  git worktree add <path> <bname> ...... create a tree at <path> from existing <bname>
  git worktree list .................... list existing work trees
  git worktree remove <tree> ........... remove work tree
  git worktree prune ................... remove stale bookkeeping files
```

## Log & Commit History
```markdown
  git log --oneline ......... shows log in single line per commit -> alias for
                              '--pretty=oneline --abbrev-commit'
  git log --graph ........... text based graph of commit history
  git log --decorate ........ decorate log with REFs

  git log -p <file> ......... show commit history + diffs for <file>
  git log --oneline <file> .. show commit history for <file> in compact format
```

## Diff & Commit Info
```markdown
  git diff <commit>..<commit> [<file>] .... show changes between two arbitrary
                                            commits. If one <commit> is omitted
                                            it is if HEAD is specified.
  git diff --name-only <commit>..<commit> . show names of files changed
  git diff -U$(wc -l <file>) <file> ....... shows complete file with diffs
                                            instead of usual diff snippets
  git diff --staged ....................... show diffs of staged files

  git show --stat <commit> ................ show files changed by <commit>
  git show <commit> [<file>] .............. show diffs for <commit>

  git show <commit>:<file> ................ show <file> at <commit>
```

## Patching
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
      <commit hash> ....... create patch with commits starting after <commit hash>

  git am <patch> ......... apply patch and create a commit for it

  git apply --stat <PATCH> ... see which files the patch would change
  git apply --check <PATCH> .. see if the patch can be applied cleanly
  git apply [-3] <PATCH> ..... apply the patch locally without creating a commit,
                               if the patch does not cleanly apply -3 allows for
                               a 3-way merge

  # eg: generate patches for each commit from initial commit on
  git format-patch -N $(git rev-list --max-parents=0 HEAD)

  # generate single patch file from a certain commit/ref
  git format-patch <COMMIT/REF> --stdout > my-patch.patch
```

## Resetting
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

## Submodules
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
  git diff --submodule ...................... show commits that are part of the
                                              submodule diff
```

## Inspection
```markdown
  git ls-tree [-r] <ref> .... show git tree for <ref>, -r to recursively ls sub-trees
  git show <obj> ............ show <obj>
  git cat-file -p <obj> ..... print content of <obj>
```

## Revision Specifier
```markdown
  HEAD ........ last commit
  HEAD~1 ...... last commit-1
  HEAD~N ...... last commit-N (linear backwards when in tree structure, check
                difference between HEAD^ and HEAD~)
  git rev-list --max-parents=0 HEAD ........... first commit
```
