---
title: "git peer-review"
draft: false
weight: 32
---

`git-repo` provides alias command `git peer-review` or `git pr` to create a code review on a single repository.


## 1. Create a code review

### 1.1 Clone

Clone remote repository to the local workspace (Ignore this step, if have local repository already):

    $ git clone https://codeup.aliyun.com/git-repo/demo.git
    $ cd demo


#### 1.2 Create a local topic branch

This step is optional. If you want to work on several different tasks at the same local workspace, you must create a topic branch for each task. Otherwise, the code review will be overwritten.

    $ git checkout -b topic1 origin/master

Note:

+ Use option `-b topic1` to create a specific branch named "topic1".
+ New branch will be created from the commit pointed by the last argument `origin/master`, and the newly created branch will setup to track it.


### 1.3 Working in the worktree

Create new commits in the worktree.


### 1.4 Publish local commits to create a new code review

Run the following command to publish local commits to remote server to create or update code review (pull request):

    $ git pr

`git pr` has many options, e.g.:

* Assign reviewers and watchers. Multiple users can be separated by commas.

        $ git pr --reviewers user1,user2 --cc user3

* Create a code review in draft mode. A draft mode code review cannot be merged.

        $ git pr --draft

If nothing changed in the local repository, will show the following error message:

    $ git pr
    NOTE: no branches ready for upload

If all local commits have already been published, execute another `git pr` will show the following message:

    $ git pr
    NOTE: no change in project . (branch topic1) since last upload
    NOTE: no branches ready for upload

If there is no tracking branch, `git pr` doesn't know the target branch to create a code review. Will show the following error message:

    $ git pr
    FATAL: upload failed: cannot find tracking branch
    
    Please run command "git branch -u <upstream>" to track a remote branch. E.g.:
    
        git branch -u origin/master

You can follow the instruction in the message to setup a remote tracking branch.


### 1.5 User interface of "git pr"

When running `git pr`, will open an editor with the following text:

    ##############################################################################
    # Step 1: Input your options for code review
    #
    # Note: Input your options below the comments and keep the comments unchanged
    ##############################################################################
    
    # [Title]       : one-line message below as the title of code review
    
    # [Description] : multiple lines of text as the description of code review
    
    # [Issue]       : multiple lines of issue IDs for cross references
    
    # [Reviewer]    : multiple lines of user names as the reviewers for code review
    
    # [Cc]          : multiple lines of user names as the watchers for code review
    
    # [Draft]       : a boolean (yes/no, or true/false) to turn on/off draft mode
    
    # [Private]     : a boolean (yes/no, or true/false) to turn on/off private mode
    
    
    ##############################################################################
    # Step 2: Select project and branches for upload
    #
    # Note: Uncomment the branches to upload, and not touch the project lines
    ##############################################################################
    
    #
    # project ./:
       branch topic1 ( 3 commit(s)) to remote branch master:
    #         03c01e211122b49fe05f48e08fd3e1dd1c52e57e
    #         ec78989440697fbc13bfbd3ea082fe9b3fc5f2d7
    #         4e599aa284ed64ba12ba1b5b06fbbd3199846434

		
The first section of the text is used for adding options for `git pr`. Lines begin with the character "#" are comments. Please do not change these comments, for `git pr` needs to find parameter names in the comments.

+ Below the line started with "# [Title]", add one-line title for the code review to be generated.
+ Below the line started with "# [Description]", add description for the code review to be generated.
+ Below the line started with "# [Issue]", add issue number for reference.
+ Below the line started with "# [Reviewer]", add reviewers (one reviewer on each line, or separated by commas).
+ Below the line started with "# [Cc]", add watchers.
+ Below the line started with "# [Draft]", input "yes" to turn on draft mode.


The second section lists branches of the current project that are ready to publish. Uncomment the branches you want to publish, and `git pr` will create code reviews for these branches.

If no branch is ready to publish (all branches are commented out), `git pr` will abort.

Save the content and quit the editor, `git pr` will continue to send local commits to remote server to create/update a code review.


### 1.6 Result of creating/updating a code review

After executing `git pr` successfully, will show message of creating/updating a code review. The following message is an example of Alibaba code platform:

    remote: +------------------------------------------------------------+
    remote: | Merge Request #7937 was created or updated.                |
    remote: | View merge request at URL:                                 |
    remote: | https://codeup.aliyun.com/git-repo/demo/merge_request/7937 |
    remote: +------------------------------------------------------------+
    To ssh://codeup.aliyun.com/git-repo/demo.git
     * [new branch]      topic1 -> refs/for/master/topic1

NOTE: in the above message, there is a URL to access the generated code review.


### 1.7 Special reference of the code review for download

A new special reference will be created inside the repository for user to access the generated code review. The special reference is not a branch or tag, and need to be fetched using a special command, e.g.:

    $ git fetch origin refs/merge-requests/7937/head
    From https://codeup.aliyun.com/git-repo/demo.git
     * branch            refs/merge-requests/7937/head -> FETCH_HEAD

Check out the fetched commit:

    $ git checkout FETCH_HEAD

NOTE: `git-repo` provides an alias command `git download` to download the specific code review.


## 2. Update a code review

If commits of the code review need to be updated, make changes in the local repository, and run the following command to update commits of the code review:

    $ git pr

## 3. Multi-user collaboration

Run `git pr` on the same repository, with the same local branch, to the same remote branch, by the same user, will update commits of the same code review (pull request), instead of creating a new one.

If a different user, such as a reviewer, wants to update the commits of a code review, how can he/she do it?

First, download the commits of the code review. Download pull request #7937 for example:

    $ git download 7937

Then, create a new local branch, such as:

    $ git checkout -b code-review

Make changes in the local repository.

Send changes of the local repository to the remote server to update the specific pull request using command like:

    $ git pr --change 7937
