# 项目 github 使用方法

1. 创建分支

分支命名需要表示出分支所有者和分支内容 （例如： zzw/add_github_instruction) 可使用以下命令创建分支

```bash
git pull # 拉取最新代码
git checkout -b zzw/add_github_instruction # 建立新分支
```

2. 提交代码

如果一个 pull request 中有多个commit都是做同一件事情，希望能合并成同一个 commit

```bash
git commit --amend
```

命令可以将这次提交合并到上次提交中

在写 commit message 的时候写上这个提交做的事情如 feat（新功能）fix（bug）doc（文档） chore（杂事）等等

这个提交可以写 doc: add github instruction

3. 提出 pull request 

在 pull request 中需要提及该提交解决的问题，最后能关联到 issue 中 (可以用 # 加上 issue 的 id 来关联 issue)。之后要@其他组员来 review 代码, 如果reviewer认为代码可以合并，在评论中写 :ship 或者 LGTM 或者 :+1 之后可以合并到主分支

4. 合并之后删除分支

```bash
git push --delete origin zzw/add_github_instruction # 删除远程分支
git branch -D zzw/add_github_instruction # 删除本地分支
```

