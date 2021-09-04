//
// Created by root on 2021/8/31.
//
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang;

class MyPPCallbacks : public PPCallbacks {
public:
  bool has_include;
  // Implement callbacks here
//  virtual void InclusionDirective(
//      SourceLocation hash_loc,
//      const Token &include_token,
//      StringRef file_name,
//      bool is_angled,
//      CharSourceRange filename_range,
//      const FileEntry *file,
//      StringRef search_path,
//      StringRef relative_path,
//      const Module *imported)
//      {
//        // do something with the include
//        has_include = true;
//        llvm::dbgs() << file_name << "\n";
//      }

  virtual bool FileNotFound(StringRef FileName, SmallVectorImpl< char > &RecoveryPath){
    llvm::dbgs() << FileName << "\n";
    return false;
  }
};

class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(CompilerInstance &CI) {
    CI.getPreprocessor().SetSuppressIncludeNotFoundError(true);
    CI.getPreprocessor().addPPCallbacks(std::make_unique<MyPPCallbacks>());
  }
};

class MyPluginASTAction : public PluginASTAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) {
    return std::make_unique<MyASTConsumer>(CI);
  }
  virtual bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) {
    return true;
  }
};

static FrontendPluginRegistry::Add<MyPluginASTAction> X("HeaderIgnore","My Plugin");