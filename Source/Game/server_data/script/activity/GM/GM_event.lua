GM_fubenhuodong_on = 0
GM_fubenhuodong_t = 0
GM_fubenhuodong_h = 0
GM_fubenhuodong_m = 0

function GM_fubenhuodong(MapID, InstanceID, TypeID, TargetID)

	GM_fubenhuodong_on = 1
	GM_fubenhuodong_t = 0
	GM_fubenhuodong_h = tonumber(os.date("%H"))
	GM_fubenhuodong_m = tonumber(os.date("%M"))

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1809)	--�·��֮����ѹ��ԣ��ɽ´���ѿ�������3Сʱ��������սԣ��ɽ´��������ɱ���조���а�ɽ��
	--��37��������Ҿ��ɻ���൱�ڵ�ǰ�ȼ���������ֵ�ľ��齱����
	msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong_t))
	GM_fubenhuodong_t=1
	msg.DispatchWorldMsgEvent(MsgID)

end


aux.RegisterItemEvent(1131101, 1, "GM_fubenhuodong")
