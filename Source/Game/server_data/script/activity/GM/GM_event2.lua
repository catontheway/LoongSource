GM_fubenhuodong2_on = 0
GM_fubenhuodong2_t = 0
GM_fubenhuodong2_h = 0
GM_fubenhuodong2_m = 0

function GM_fubenhuodong2(MapID, InstanceID, TypeID, TargetID)

	GM_fubenhuodong2_on = 1
	GM_fubenhuodong2_t = 0
	GM_fubenhuodong2_h = tonumber(os.date("%H"))
	GM_fubenhuodong2_m = tonumber(os.date("%M"))

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1811)	--�·��֮����ѹ�������̳���ѿ�������3Сʱ��������ս�����̳��������ɱ���조�����ͷ��צ��
	--��37��������Ҿ��ɻ���൱�ڵ�ǰ�ȼ���������ֵ�ľ��齱����
	msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong2_t))
	GM_fubenhuodong2_t=1
	msg.DispatchWorldMsgEvent(MsgID)

end


aux.RegisterItemEvent(1131102, 1, "GM_fubenhuodong2")
