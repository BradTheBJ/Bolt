#include "bolt/parser.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

LLVMContext context;
auto module = std::make_unique<Module>("BoltModule", context);
IRBuilder<> builder(context);

FunctionType* printfType = FunctionType::get(builder.getInt32Ty(), true);
Function* printfFunc = Function::Create(printfType, Function::ExternalLinkage, "printf", module.get());

Value* codegenNumber(NumberNode* node) {
    return ConstantInt::get(builder.getInt32Ty(), node->value);
}

Value* codegenPrint(PrintNode* node) {
    NumberNode* numNode = dynamic_cast<NumberNode*>(node->expr.get());
    if (!numNode) return nullptr;
    Value* val = codegenNumber(numNode);
    Value* formatStr = builder.CreateGlobalStringPtr("%d\n");
    return builder.CreateCall(printfFunc, {formatStr, val});
}

void codeGen(std::unique_ptr<ASTNode>& astRoot, const std::string& llFilename) {
    FunctionType* mainType = FunctionType::get(builder.getInt32Ty(), false);
    Function* mainFunc = Function::Create(mainType, Function::ExternalLinkage, "main", module.get());
    BasicBlock* entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    PrintNode* printNode = dynamic_cast<PrintNode*>(astRoot.get());
    if (printNode) {
        codegenPrint(printNode);
    }

    builder.CreateRet(ConstantInt::get(builder.getInt32Ty(), 0));

    std::error_code EC;
    llvm::raw_fd_ostream file(llFilename, EC, llvm::sys::fs::OpenFlags());
    module->print(file, nullptr);
    file.close();
}
