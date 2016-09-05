#include "DebugView.hpp"

DebugView::DebugView(QWidget* parent) :
  QPlainTextEdit(parent)
{
  setReadOnly(true);
  setMaximumBlockCount(1000);
}

void DebugView::print(const QString& str)
{
  moveCursor(QTextCursor::End);
  insertPlainText(str);
  moveCursor(QTextCursor::End);
}
