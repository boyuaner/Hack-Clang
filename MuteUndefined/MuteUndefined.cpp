//
// Created by root on 2021/9/3.
//
#include "clang/AST/RecursiveASTVisitor.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <sstream>

using namespace clang;
Rewriter TheReWriter;

class FindFuncVisitor
    : public RecursiveASTVisitor<FindFuncVisitor> {
public:
  explicit FindFuncVisitor(ASTContext* Context)
  : Context(Context) {}
  bool VisitCallExpr(CallExpr *Declaration) {
    // For debugging, dumping the AST nodes will show which nodes are already
    // being visited.
    Declaration->dump();
    if(Declaration->getDirectCallee() != NULL){
      FunctionDecl* f = Declaration->getDirectCallee();
//      llvm::dbgs() << ->getNameInfo().getName().getAsString() <<"\n";
      if(!f->isDefined() && !f->hasPrototype()){
        llvm::dbgs() << "hello\n";
        std::stringstream SSBefore;
        SSBefore << "/* before \n";
        SourceLocation ST = Declaration->getBeginLoc();
        TheReWriter.InsertText(ST, SSBefore.str(),true, true);
        std::stringstream SSAfter;
        SSAfter << "after */ \n";
        ST = Declaration->getEndLoc().getLocWithOffset(2);
        TheReWriter.InsertText(ST, SSAfter.str(),true, true);
      }
    }

    // The return value indicates whether we want the visitation to proceed.
    // Return false to stop the traversal of the AST.
    return true;
  }
private:
  ASTContext *Context;
//  Rewriter TheRewriter;
};
class FindFuncConsumer : public clang::ASTConsumer {
public:
  explicit FindFuncConsumer(ASTContext* Context)
  : Visitor(Context) {}
//  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
//    // Traversing the translation unit decl via a RecursiveASTVisitor
//    // will visit all nodes in the AST.
//    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
//  }
  virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
    for(auto& b : DR){
      Visitor.TraverseDecl(b);
    }
//    TheReWriter.getEditBuffer(TheReWriter.getSourceMgr().getMainFileID()).write(llvm::outs());
    llvm::dbgs() << "write backing\n";
    if(TheReWriter.overwriteChangedFiles()){
      llvm::dbgs() << "failed\n";
    }
    return true;
  }
private:
  // A RecursiveASTVisitor implementation.
  FindFuncVisitor Visitor;
};
class MuteUndefinedASTAction : public PluginASTAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) {
    TheReWriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<FindFuncConsumer>(&CI.getASTContext());
//    return std::make_unique<FindFuncConsumer>(TheReWriter);
  }
//  void EndSourceFileAction() override {
//    llvm::dbgs() <<"end\n";
//    TheReWriter.getEditBuffer(TheReWriter.getSourceMgr().getMainFileID()).write(llvm::outs());
//    TheReWriter.overwriteChangedFiles();
//  }
  virtual bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) {
    return true;
  }
};

static FrontendPluginRegistry::Add<MuteUndefinedASTAction> Y("MuteUndefined","My Plugin");