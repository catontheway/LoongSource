--��ѯ��������
function I2610158_UseItem(MapID, InstanceID, TypeID, TargetID)
	for k=3600020, 3600025 do
		if MapID == tiangongxingguan[k].MapCrc then
			--��ѯ�Ƿ��д�������
			aux.WriteLog(tiangongxingguan[k].IsDead)
			if false == tiangongxingguan[k].IsDead then
				--��ʾ��ҡ���####��####�����꣩����������֮��������
				local MsgID = msg.BeginMsgEvent()
				--msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 71, 4)
				msg.AddMsgEvent(MsgID, 9, tiangongxingguan[k].x)
				msg.AddMsgEvent(MsgID, 9, tiangongxingguan[k].z)
				msg.DispatchRoleMsgEvent(TargetID, MsgID)
				return
			end
		end
	end
	--��ʾ��ҡ�û�з����κ�����֮������������ȫ���ط���硣��
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 71, 2)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

aux.RegisterItemEvent(2610158, 1, "I2610158_UseItem")
