#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QLineEdit>
#include "KeyboardWidget.hpp"


static const int KEY_ROW1_START_IDX = 0;
static const int KEY_ROW1_END_IDX = 10;
static const int KEY_ROW2_START_IDX = 10;
static const int KEY_ROW2_END_IDX = 19;
static const int KEY_SHIFT_IDX = 19;
static const int KEY_ROW3_START_IDX = 20;
static const int KEY_ROW3_END_IDX = 27;
static const int KEY_BACK_IDX = 27;
static const int KEY_NUM_MODE_IDX = 28;
static const int KEY_SPEC_MODE_IDX = 29;
static const int KEY_SPACE_IDX = 30;
static const int KEY_OK_IDX = 31;

KeyboardWidget::KeyboardWidget(QWidget * parent) : QWidget(parent)
{
        QVBoxLayout * mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(4, 4, 4, 4);
        mainLayout->setSpacing(4);

        // row 1
        QHBoxLayout * row1 = new QHBoxLayout(this);
        row1->setSpacing(4);
        for (int i = KEY_ROW1_START_IDX; i < KEY_ROW1_END_IDX; i++) { row1->addWidget(makeKey(i)); }
        mainLayout->addLayout(row1);

        // row 2
        QHBoxLayout * row2 = new QHBoxLayout(this);
        row2->setSpacing(4);
        for (int i = KEY_ROW2_START_IDX; i < KEY_ROW2_END_IDX; i++) { row2->addWidget(makeKey(i)); }
        mainLayout->addLayout(row2);

        // row 3
        QHBoxLayout * row3 = new QHBoxLayout(this);
        row3->setSpacing(4);
        {
                // SHIFT
                QPushButton * btn = makeKey(KEY_SHIFT_IDX);
                btn->setText("^");
                btn->setStyleSheet("background-color: gray; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row3->addWidget(btn);
        }
        for (int i = KEY_ROW3_START_IDX; i < KEY_ROW3_END_IDX; i++) {
                QPushButton * btn = makeKey(i);
                row3->addWidget(btn);
        }
        {
                // BACK
                QPushButton * btn = makeKey(KEY_BACK_IDX);
                btn->setText("<");
                btn->setStyleSheet("background-color: red; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row3->addWidget(btn);
        }
        mainLayout->addLayout(row3);

        // row 4
        QHBoxLayout * row4 = new QHBoxLayout(this);
        row4->setSpacing(4);
        row4->addStretch();
        {
                // [123]
                QPushButton * btn = makeKey(KEY_NUM_MODE_IDX);
                btn->setStyleSheet("background-color: gray; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row4->addWidget(btn);
        }
        {
                // [ÄÖÜ]
                QPushButton * btn = makeKey(KEY_SPEC_MODE_IDX);
                btn->setStyleSheet("background-color: gray; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row4->addWidget(btn);
        }
        {
                // SPACE
                QPushButton * btn = makeKey(KEY_SPACE_IDX);
                btn->setText("_");
                btn->setStyleSheet("background-color: gray; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row4->addWidget(btn);
        }
        {
                // OK
                QPushButton * btn = makeKey(KEY_OK_IDX);
                btn->setText("OK");
                btn->setStyleSheet("background-color: green; font-weight: bold; "
                                   "font-size: 18px; border-radius: 5px; "
                                   "min-width: 55px; min-height: 45px;");
                row4->addWidget(btn);
        }
        row4->addStretch();
        mainLayout->addLayout(row4);
        updateLabels();
        setVisible(false);
}

void KeyboardWidget::attachInput(QLineEdit * input)
{
        mAttachedInputs.append(input);
        if (mAttachedInputs.size() == 1) {
                connect(static_cast<QApplication *>(QApplication::instance()),
                        &QApplication::focusChanged, this, &KeyboardWidget::onFocusChanged);
        }
}

QLineEdit * KeyboardWidget::currentInput() const
{
        QWidget * focused = QApplication::focusWidget();
        for (QLineEdit * input : mAttachedInputs) { if (input == focused) { return input; } }
        return mAttachedInputs.isEmpty() ? nullptr : mAttachedInputs.last();
}

QPushButton * KeyboardWidget::makeKey(int index)
{
        QPushButton * btn = new QPushButton("", this);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setStyleSheet("background-color: darkBlue; color: white; font-size: 18px; "
                           "border-radius: 5px; min-width: 50px, min-height: 45px;");
        connect(btn, &QPushButton::clicked, this, [this, index]() { onKeyClicked(index); });
        mKeyButtons.append(btn);
        return btn;
}

void KeyboardWidget::updateLabels()
{
        // row 1
        QStringList row1 = mNumericMode ? mKeyRow1Numeric : mSpecialCharsMode ? mKeyRow1Special : mKeyRow1Default;
        for (int i = KEY_ROW1_START_IDX; i < KEY_ROW1_END_IDX; i++) { setKeyText(i, row1[i - KEY_ROW1_START_IDX]); }

        // row 2
        QStringList row2 = mSpecialCharsMode ? mKeyRow2Special : mKeyRow2Default;
        for (int i = KEY_ROW2_START_IDX; i < KEY_ROW2_END_IDX; i++) { setKeyText(i, row2[i - KEY_ROW2_START_IDX]); }

        // row 3 letters
        for (int i = KEY_ROW3_START_IDX; i < KEY_ROW3_END_IDX; i++) { setKeyText(i, mKeyRow3Default[i - KEY_ROW3_START_IDX]); }

        // control buttons
        mKeyButtons[KEY_SHIFT_IDX]->setText(mShift ? "°" : "^");
        mKeyButtons[KEY_BACK_IDX]->setText("<");
        mKeyButtons[KEY_NUM_MODE_IDX]->setText(mNumericMode ? "abc" : "123");
        mKeyButtons[KEY_SPEC_MODE_IDX]->setText(mSpecialCharsMode ? "abc" : "ÄÖÜ");
        mKeyButtons[KEY_SPACE_IDX]->setText("_");
        mKeyButtons[KEY_OK_IDX]->setText("OK");
}

void KeyboardWidget::setKeyText(int idx, const QString & text)
{
        if (text.length() == 1 && text[0].isLetter() && !mNumericMode) {
                mKeyButtons[idx]->setText(mShift ? text.toUpper() : text.toLower());
        } else { mKeyButtons[idx]->setText(text); }
}

void KeyboardWidget::onKeyClicked(int index)
{
        QLineEdit * input = currentInput();
        if (!input) { return; }

        if (index == KEY_SHIFT_IDX) { onShiftClicked(); return; }
        if (index == KEY_BACK_IDX) { onBackspaceClicked(); return; }
        if (index == KEY_OK_IDX) { onDoneClicked(); return; }
        if (index == KEY_SPACE_IDX) { input->insert(" "); return; }

        if (index == KEY_NUM_MODE_IDX) {
                mNumericMode = !mNumericMode;
                mSpecialCharsMode = false;
                mShift = false;
                updateLabels();
                return;
        }
        if (index == KEY_SPEC_MODE_IDX) {
                mSpecialCharsMode = !mSpecialCharsMode;
                mNumericMode = false;
                mShift = false;
                updateLabels();
                return;
        }

        input->insert(mKeyButtons[index]->text());
        if (mShift) { onShiftClicked(); }
}

void KeyboardWidget::onShiftClicked()
{
        mShift = !mShift;
        updateLabels();
}

void KeyboardWidget::onBackspaceClicked()
{
        QLineEdit * input = currentInput();
        if (input) input->backspace();
}

void KeyboardWidget::onDoneClicked()
{
        QLineEdit * input = currentInput();
        if (input) input->clearFocus();
        hide();
}

void KeyboardWidget::onFocusChanged(QWidget * old, QWidget * now)
{
        Q_UNUSED(old);
        setVisible(mAttachedInputs.contains(qobject_cast<QLineEdit *>(now)));
}
