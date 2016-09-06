#include "DebugView.hpp"

DebugView::DebugView(QWidget* parent) :
  QPlainTextEdit(parent)
{
  setReadOnly(true);
  setMaximumBlockCount(1000);
}

void DebugView::post_cycle(const SimulatorCycleBundle& bundle)
{
  moveCursor(QTextCursor::End);
  insertPlainText(QString::fromStdString(bundle.debug_output));
  moveCursor(QTextCursor::End);
}
