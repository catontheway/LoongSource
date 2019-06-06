--RoleDataType["Christmas_Box"]  = 162  ��¼��ҵ����Ƿ��ѽ�����й���ʥ������
--6010808 �յ��Ų����
--6010809 �Ų����



function ChristmasTree_OnTalk(MapID,InstanceID,TargetID,TargetTypeID,RoleID,TalkIndex)

	local Christmas_Box = role.GetRoleScriptData(RoleID,1,RoleDataType["Christmas_Box"]) --��ȡ��ҵ���Ŀ�����Ƿ��ѹ�������
	local curday = tonumber(os.date("%j"))	 	-- ��ȡ��������ǰ����
	local curhour = tonumber(os.date("%H"))  	-- ��ȡ��������ǰСʱ��
	local Shengdan = tonumber(os.date("%j"))   --��ȡ��������ǰ���ڼ�¼

	if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 4401)	-- "��ÿ����Թ���һ����У�ʥ�����˻�����ö���Ŷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 4402)	-- "���Ͽյ����"
			msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
			msg.AddMsgEvent(MsgID, 1, 4403)	-- "��ȡ���"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if curhour < 21 and Shengdan ~= Christmas_Box then
			if role.GetRoleItemNum(RoleID, 6010808) < 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 4404)		--������пյ���У��뽫���Ž����ı��������ҡ�
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 6010808, 1, 420)		--ɾ����ұ����ڵĿպ���
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Christmas_Box"], curday) --������ҵĹҺ��Ӽ�¼
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 4412)		--��������Ѿ�������ʥ�����ϣ��������ĵȴ�ʥ�����˸��������ľ�ϲ�ɣ�
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif Christmas_Box == Shengdan and curhour < 21 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 4405)		--�������Ѿ�������й���ʥ�������ˣ�����21:00-24:00֮������ȡ����ɣ�
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif curhour >= 21 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 4408)		--ֻ����00��00-21��00֮��ſ��Խ�����й�������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 5 then
		if Christmas_Box ~= Shengdan and curhour >= 21 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 4407)		--������û�н�����й���ʥ������Ŷ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Christmas_Box == Shengdan and curhour >= 21 then
			if role.GetBagFreeSize(RoleID) > 0 then		-- �ж�����Ƿ����㹻�����ռ�
				role.AddRoleItem(MapID, InstanceID, RoleID, 6010809, 1, -1, 8, 420)	--����ұ������������
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Christmas_Box"], 0) --����ҵĹҺ��Ӽ�¼���
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 4409)		--���ı����ռ䲻�㣡
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif curhour < 21 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 4406)		--��û����ȡʱ�䣬������21:00-24:00֮������ȡ����ɣ�
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

aux.RegisterCreatureEvent(4900714, 7, "ChristmasTree_OnTalk")
