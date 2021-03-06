/*
 * Copyright (C) 2012 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ewk_navigation_policy_decision_private_h
#define ewk_navigation_policy_decision_private_h

#include "WKAPICast.h"
#include "WKBase.h"
#include "WKEinaSharedString.h"
#include "WKEvent.h"
#include "WKFramePolicyListener.h"
#include "WKPageLoadTypes.h"
#include "WKRetainPtr.h"
#include "ewk_url_request_private.h"
#include <wtf/PassRefPtr.h>

/**
 * \struct  EwkNavigationPolicyDecision
 * @brief   Contains the navigation policy decision data.
 */
class EwkNavigationPolicyDecision : public EwkObject {
public:
    EWK_OBJECT_DECLARE(EwkNavigationPolicyDecision)

    ~EwkNavigationPolicyDecision();

    static PassRefPtr<EwkNavigationPolicyDecision> create(WKFrameNavigationType navigationType, WKEventMouseButton mouseButton, WKEventModifiers modifiers, WKURLRequestRef requestRef, const char* frameName, WKFramePolicyListenerRef listener)
    {
        return adoptRef(new EwkNavigationPolicyDecision(listener, navigationType, mouseButton, modifiers, EwkUrlRequest::create(requestRef), frameName));
    }

    void accept();
    void reject();
    void download();

    Ewk_Navigation_Type navigationType() const;
    Ewk_Event_Mouse_Button mouseButton() const;
    Ewk_Event_Modifiers modifiers() const;
    const char* frameName() const;
    EwkUrlRequest* request() const;

private:
    EwkNavigationPolicyDecision(WKFramePolicyListenerRef listener, WKFrameNavigationType navigationType, WKEventMouseButton mouseButton, WKEventModifiers modifiers, PassRefPtr<EwkUrlRequest> request, const char* frameName);

    WKRetainPtr<WKFramePolicyListenerRef> m_listener;
    bool m_actedUponByClient;
    WKFrameNavigationType m_navigationType;
    WKEventMouseButton m_mouseButton;
    WKEventModifiers m_modifiers;
    RefPtr<EwkUrlRequest> m_request;
    WKEinaSharedString m_frameName;
};

#endif // ewk_navigation_policy_decision_private_h
