/*
 * ASTPrinter.cpp
 * 
 * This file is part of the "HLSL Translator" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTPrinter.h"
#include "HLSLTree.h"


namespace HTLib
{


void ASTPrinter::DumpAST(Program* program, Logger& log)
{
    log_ = &log;
    Visit(program);
}


/*
 * ======= Private: =======
 */

class ScopedIndent
{
    public:
        ScopedIndent(Logger& log) :
            log_{ log }
        {
            log_.IncIndent();
        }
        ~ScopedIndent()
        {
            log_.DecIndent();
        }
    private:
        Logger& log_;
};

#define SCOPED_INDENT ScopedIndent indent(*log_)

/* ------- Visit functions ------- */

#define IMPLEMENT_VISIT_PROC(className) \
    void ASTPrinter::Visit##className(className* ast, void* args)

IMPLEMENT_VISIT_PROC(Program)
{
    Print(ast, "Program");
    SCOPED_INDENT;

    for (auto& globDecl : ast->globalDecls)
        Visit(globDecl);
}

IMPLEMENT_VISIT_PROC(CodeBlock)
{
    Print(ast, "CodeBlock");
    SCOPED_INDENT;

    for (auto& stmnt : ast->stmnts)
        Visit(stmnt);
}

IMPLEMENT_VISIT_PROC(BufferDeclIdent)
{
    Print(ast, "BufferDeclIdent", ast->ident);
}

IMPLEMENT_VISIT_PROC(FunctionCall)
{
    Print(ast, "FunctionCall");
    SCOPED_INDENT;

    Visit(ast->name);
    for (auto& arg : ast->arguments)
        Visit(arg);
}

IMPLEMENT_VISIT_PROC(Structure)
{
    Print(ast, "Structure");
    SCOPED_INDENT;

    for (auto& member : ast->members)
        Visit(member);
}

IMPLEMENT_VISIT_PROC(SwitchCase)
{
    Print(ast, "SwitchCase");
    SCOPED_INDENT;

    for (auto& stmnt : ast->stmnts)
        Visit(stmnt);
}

/* --- Global declarations --- */

IMPLEMENT_VISIT_PROC(FunctionDecl)
{
    Print(ast, "FunctionDecl", ast->name);
    SCOPED_INDENT;

    for (auto& attrib : ast->attribs)
        Visit(attrib);
    Visit(ast->codeBlock);
}

IMPLEMENT_VISIT_PROC(UniformBufferDecl)
{
    Print(ast, "UniformBufferDecl", ast->name + " (" + ast->bufferType + ")");
    SCOPED_INDENT;
    
    for (auto& member : ast->members)
        Visit(member);
}

IMPLEMENT_VISIT_PROC(TextureDecl)
{
    Print(ast, "TextureDecl");
    SCOPED_INDENT;

    for (auto& name : ast->names)
        Visit(name);
}

IMPLEMENT_VISIT_PROC(SamplerDecl)
{
    Print(ast, "SamplerDecl");
    SCOPED_INDENT;

    for (auto& name : ast->names)
        Visit(name);
}

IMPLEMENT_VISIT_PROC(StructDecl)
{
    Print(ast, "StructDecl");
    SCOPED_INDENT;

    Visit(ast->structure);
}

IMPLEMENT_VISIT_PROC(DirectiveDecl)
{
    Print(ast, "DirectiveDecl", ast->line);
}

/* --- Statements --- */

IMPLEMENT_VISIT_PROC(NullStmnt)
{
    Print(ast, "NullStmnt");
}

IMPLEMENT_VISIT_PROC(DirectiveStmnt)
{
    Print(ast, "DirectiveStmnt", ast->line);
}

IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
{
    Print(ast, "CodeBlockStmnt");
    SCOPED_INDENT;
    
    Visit(ast->codeBlock);
}

IMPLEMENT_VISIT_PROC(ForLoopStmnt)
{
    Print(ast, "ForLoopStmnt");
    SCOPED_INDENT;

    Visit(ast->initSmnt);
    Visit(ast->condition);
    Visit(ast->iteration);
    Visit(ast->bodyStmnt);
}

IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
{
    Print(ast, "WhileLoopStmnt");
    SCOPED_INDENT;

    Visit(ast->condition);
    Visit(ast->bodyStmnt);
}

IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
{
    Print(ast, "DoWhileLoopStmnt");
    SCOPED_INDENT;

    Visit(ast->bodyStmnt);
    Visit(ast->condition);
}

IMPLEMENT_VISIT_PROC(IfStmnt)
{
    Print(ast, "IfStmnt");
    SCOPED_INDENT;

    Visit(ast->condition);
    Visit(ast->bodyStmnt);
    Visit(ast->elseStmnt);
}

IMPLEMENT_VISIT_PROC(ElseStmnt)
{
    Print(ast, "ElseStmnt");
    SCOPED_INDENT;

    Visit(ast->bodyStmnt);
}

IMPLEMENT_VISIT_PROC(SwitchStmnt)
{
    Print(ast, "SwitchStmnt");
    SCOPED_INDENT;

    Visit(ast->selector);
    for (auto& switchCase : ast->cases)
        Visit(switchCase);
}

IMPLEMENT_VISIT_PROC(VarDeclStmnt)
{
    Print(ast, "VarDeclStmnt");
    SCOPED_INDENT;

    for (auto decl : ast->varDecls)
        Visit(decl);
}

IMPLEMENT_VISIT_PROC(AssignStmnt)
{
    Print(ast, "AssignStmnt");
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(ExprStmnt)
{
    Print(ast, "ExprStmnt");
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(FunctionCallStmnt)
{
    Print(ast, "FunctionCallStmnt");
    SCOPED_INDENT;

    Visit(ast->call);
}

IMPLEMENT_VISIT_PROC(ReturnStmnt)
{
    Print(ast, "ReturnStmnt");
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(StructDeclStmnt)
{
    Print(ast, "StructDeclStmnt");
    SCOPED_INDENT;

    Visit(ast->structure);
}

IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
{
    Print(ast, "CtrlTransferStmnt", ast->instruction);
}

/* --- Expressions --- */

IMPLEMENT_VISIT_PROC(ListExpr)
{
    Print(ast, "ListExpr");
    SCOPED_INDENT;

    Visit(ast->firstExpr);
    Visit(ast->nextExpr);
}

IMPLEMENT_VISIT_PROC(LiteralExpr)
{
    Print(ast, "LiteralExpr", ast->literal);
}

IMPLEMENT_VISIT_PROC(TypeNameExpr)
{
    Print(ast, "TypeNameExpr", ast->typeName);
}

IMPLEMENT_VISIT_PROC(TernaryExpr)
{
    Print(ast, "TernaryExpr");
    SCOPED_INDENT;

    Visit(ast->condition);
    Visit(ast->ifExpr);
    Visit(ast->elseExpr);
}

IMPLEMENT_VISIT_PROC(BinaryExpr)
{
    Print(ast, "BinaryExpr", ast->op);
    SCOPED_INDENT;

    Visit(ast->lhsExpr);
    Visit(ast->rhsExpr);
}

IMPLEMENT_VISIT_PROC(UnaryExpr)
{
    Print(ast, "UnaryExpr", ast->op);
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(PostUnaryExpr)
{
    Print(ast, "PostUnaryExpr", ast->op);
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(FunctionCallExpr)
{
    Print(ast, "FunctionCallExpr");
    SCOPED_INDENT;

    Visit(ast->call);
}

IMPLEMENT_VISIT_PROC(BracketExpr)
{
    Print(ast, "BracketExpr");
    SCOPED_INDENT;

    Visit(ast->expr);
}

IMPLEMENT_VISIT_PROC(CastExpr)
{
    Print(ast, "CastExpr");
    SCOPED_INDENT;

    Visit(ast->typeExpr);
}

IMPLEMENT_VISIT_PROC(VarAccessExpr)
{
    Print(ast, "VarAccessExpr");
    SCOPED_INDENT;

    Visit(ast->varIdent);
    Visit(ast->assignExpr);
}

IMPLEMENT_VISIT_PROC(InitializerExpr)
{
    Print(ast, "InitializerExpr");
    SCOPED_INDENT;

    for (auto& expr : ast->exprs)
        Visit(expr);
}

/* --- Variables --- */

IMPLEMENT_VISIT_PROC(PackOffset)
{
    auto info = ast->registerName;

    if (!ast->vectorComponent.empty())
        info += " (" + ast->vectorComponent + ")";

    Print(ast, "PackOffset", info);
}

IMPLEMENT_VISIT_PROC(VarSemantic)
{
    auto info = ast->semantic;

    if (!ast->registerName.empty())
        info += " (" + ast->registerName + ")";

    Print(ast, "VarSemantic", info);
    SCOPED_INDENT;

    Visit(ast->packOffset);
}

IMPLEMENT_VISIT_PROC(VarType)
{
    Print(ast, "VarType", ast->baseType);
    SCOPED_INDENT;

    Visit(ast->structType);
}

IMPLEMENT_VISIT_PROC(VarIdent)
{
    Print(ast, "VarIdent", ast->ident);
    SCOPED_INDENT;

    for (auto& index : ast->arrayIndices)
        Visit(index);
    Visit(ast->next);
}

IMPLEMENT_VISIT_PROC(VarDecl)
{
    Print(ast, "VarDecl", ast->name);
    SCOPED_INDENT;

    for (auto& dim : ast->arrayDims)
        Visit(dim);
    for (auto& semantic : ast->semantics)
        Visit(semantic);
    Visit(ast->initializer);
}

#undef IMPLEMENT_VISIT_PROC
#undef SCOPED_INDENT

/* --- Helper functions --- */

void ASTPrinter::Print(AST* ast, const std::string& astName, const std::string& info)
{
    std::string msg = astName + " (" + ast->pos.ToString() + ")";

    if (!info.empty())
        msg += " \"" + info + "\"";

    log_->Info(msg);
}

void ASTPrinter::IncIndent()
{
    log_->IncIndent();
}

void ASTPrinter::DecIndent()
{
    log_->DecIndent();
}


} // /namespace HTLib



// ================================================================================