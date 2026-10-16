#ifndef CALCULATIONCONTROL_H
#define CALCULATIONCONTROL_H

#pragma once

#include <QObject>

namespace fosd{
namespace Calculation{

namespace detail{
    
}

class CalculationControl final : public QObject{
    Q_OBJECT
public:
    explicit CalculationControl(QObject *parent = nullptr);
    ~CalculationControl() override;


};

}
}


#endif