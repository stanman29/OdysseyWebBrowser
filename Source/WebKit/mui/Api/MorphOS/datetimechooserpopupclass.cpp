/*
 * Copyright 2012 Fabien Coeurjoly <fabien.coeurjoly@wanadoo.fr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "GraphicsContext.h"
#include "DateTimeChooser.h"
#include "DateTimeChooserClient.h"
#include "DateTimeChooserController.h"

#include "wtf/text/CString.h"

#include <Calltips_mcc.h>
#include <clib/debug_protos.h>

#include "gui.h"

#define D(x)

using namespace WebCore;

/******************************************************************
 * autofillpopupclass
 *****************************************************************/

struct Data
{
	Object *bt_use;
	Object *str_date;
	Object *source;
	DateTimeChooserController *controller;
};

DEFNEW
{
	Object *source                        = (Object *) GetTagData(MA_DateTimeChooserPopup_Source, NULL, msg->ops_AttrList);
	DateTimeChooserController *controller = (DateTimeChooserController *) GetTagData(MA_DateTimeChooserPopup_Controller, NULL, msg->ops_AttrList);

	if(controller)
	{
		Object *str_date, *bt_use;
		IntRect rect = controller->parameters().anchorRectInRootView;
		struct Rect32 r = { _mleft(source) + rect.x(), _mtop(source) + rect.y() + rect.height(), _mleft(source) + rect.maxX() + 200, _mtop(source) + rect.y() + rect.height() + 400 };

		obj = (Object *) DoSuperNew(cl, obj,
			MUIA_Window_RootObject,
				VGroup, //MUIA_FixWidth, rect.width(),
					Child, str_date = (Object *) MakeString("", FALSE),
					Child, bt_use = (Object *) MakeButton(GSI(MSG_PREFSWINDOW_USE)),
					End,
			MUIA_Calltips_Source, source,
			MUIA_Calltips_Rectangle, &r,
			TAG_MORE, INITTAGS
		);

		if (obj)
		{
			GETDATA;
			data->bt_use = bt_use;
			data->str_date = str_date;
			data->controller = controller;
			data->source = source;

			set(data->str_date, MUIA_String_Contents, controller->parameters().currentValue.utf8().data());
			DoMethod(data->bt_use, MUIM_Notify, MUIA_Pressed, FALSE, obj, 3, MM_DateTimeChooserPopup_DidSelect, TRUE);
		}
	}

	return ((IPTR)obj);
}

DEFDISP
{
	GETDATA;

	if(data->controller)
	{
		data->controller->didEndChooser();
		data->controller = NULL;
	}

	return DOSUPER;
}

DEFSMETHOD(DateTimeChooserPopup_DidSelect)
{
	GETDATA;

	char * value = (char *) getv(data->str_date, MUIA_String_Contents);
	value = "08/05/2014";

	data->controller->didChooseValue(String(value));

	if(msg->close)
	{
		DoMethod(app, MUIM_Application_PushMethod, data->source, 1, MM_OWBBrowser_DateTimeChooser_HidePopup);
	}

	return 0;
}

BEGINMTABLE
DECNEW
DECDISP
DECSMETHOD(DateTimeChooserPopup_DidSelect)
ENDMTABLE

DECSUBCLASS_NC(MUIC_Calltips, datetimechooserpopupclass)

