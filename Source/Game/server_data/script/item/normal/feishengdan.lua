function I3304015_OnUse(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.SetSoarValue(TargetID, 1)
	role.OpenTrain(TargetID, 87)
	role.SigTitleEvent(TargetID, 282)
	unit.AddBuff(MapID, InstanceID, TargetID, 9431201, TargetID)
	local k = act.GetActScriptData(99, 1, 6) + 1
	if k <= 5 then
	    act.SetActScriptData(99, 1, k, TargetID)
		--act.SetActScriptData(99, 1, k+6, 10)
	end
	act.SetActScriptData(99, 1, 6, k)
	act.SaveActScriptData(99)
	local MsgID = msg.BeginMsgEvent()		-- XXX�����˷���������Ϊ����������Xλ��������ң�
	msg.AddMsgEvent(MsgID, 100, 3112)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 9, k)
	msg.DispatchWorldMsgEvent(MsgID)
	local MsgID = msg.BeginMsgEvent()		-- ��ϲ���ɹ�����������һ��ȫ�µĿ�ʼ�������Զ��Լ���װ�������µ�ǿ�����������������������Խ��������ս�Եĸ��������Ϲ��޻ض�Ѩ����ħ�磡
	msg.AddMsgEvent(MsgID, 71, 3113)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --������Ϣ
	if k <= 200 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100122)    --XXX�����˷���������Ϊ����������Xλ��������ң�
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 9, k)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
		--[[local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100122)    --XXX�����˷���������Ϊ����������Xλ��������ң�
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 9, k)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)]]
		local MsgID = msg.BeginMsgEvent()		-- XXX�����˷���������Ϊ����������Xλ��������ң�
		msg.AddMsgEvent(MsgID, 101, 3111)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 9, k)
		msg.DispatchWorldMsgEvent(MsgID)
		--[[local MsgID = msg.BeginMsgEvent()		-- XXX�����˷���������Ϊ����������Xλ��������ң�
		msg.AddMsgEvent(MsgID, 101, 3111)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 9, k)
		msg.DispatchWorldMsgEvent(MsgID)]]
	end

end

aux.RegisterItemEvent(3304015, 1, "I3304015_OnUse")

