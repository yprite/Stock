/*
 * SaListEditEventListener.h
 *
 *  Created on: Sep 8, 2017
 *      Author: devbin
 */

#ifndef SALISTEDITEVENTLISTENER_H_
#define SALISTEDITEVENTLISTENER_H_

class SaListEditEventListener
{
public:
    SaListEditEventListener();
    virtual ~SaListEditEventListener();

    virtual void onStartEdit() = 0;
    virtual void onStopEdit() = 0;
    virtual void onUpdateEdit() = 0;

protected:
    friend class SaListEditEventManager;
};

#endif /* SALISTEDITEVENTLISTENER_H_ */
