

--[[
?	�����������������
?	��ҵ������紫���˺󣬵����Ի����棺
?	�Ի����ݣ�
������������ɽɽ������ʮ����֮һ�������������ļ��ѳ�������Ϊ��������������̾�컯Ū�ˣ�ʮ��֮�䣬����һ�ɲ���ħ����¾��������������������Ԫ���ţ�������Ů��ħ���ƻ���Ȼ��ħ��߱ħ֮�ˣ����������ߡ���������ĩ���ƽ٣��ɱ����ɱ�������
����������޷��������װ����
?	ѡ��һ��������ȥ�����
?	���������жϣ���Ҫ�������һö�����ͨ���û�е��ߵĻ���������ʾȱ�������ͨ���
]]--


function ZZJ_chuansongren_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4217) -- "������������ɽɽ������ʮ����֮һ�������������ļ��ѳ�������Ϊ��������������̾�컯Ū�ˣ�ʮ��֮�䣬����һ�ɲ���ħ����¾��������������������Ԫ���ţ�������Ů��ħ���ƻ���Ȼ��ħ��߱ħ֮�ˣ����������ߡ���������ĩ���ƽ٣��ɱ����ɱ�������\n����������޷��������װ����"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 4218)	-- "������ȥ����磨��Ҫ����һö�����ͨ���"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	if TalkIndex == 4 then
		local Item = role.GetRoleItemNum(RoleID,3200488)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID) --��ҵȼ�
		if level < 100 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 82)   --�ȼ����㣬�޷�����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			if Item <= 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4219)   --ȱ�ٵ���
				msg.AddMsgEvent(MsgID, 4, 3200488) --��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				role.RoleGotoNewMap(MapID, InstanceID,RoleID,2291723889,526,160,554)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3200488, 1, 420)
			end
		end
	end

end


aux. RegisterCreatureEvent(4902003, 7, "ZZJ_chuansongren_On_Talk")
































