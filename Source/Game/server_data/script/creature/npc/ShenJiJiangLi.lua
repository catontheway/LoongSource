--�����ά���˽ű�

function n_ShenJiJiangLi(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local ShenJiPoint = role.GodMiraclePoints(MapID, InstanceID, RoleID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	local Last_Recv_D = role.GetRoleScriptData(RoleID, 1, RoleDataType["Last_Recv_D"])

	if TalkIndex == -1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2239)
		msg.AddMsgEvent(MsgID, 9, ShenJiPoint)
		--ս��
		--    ������ʹ����������ս������õ��񼣻����ڴ˴��һ�������
		--����һ���Ҫ�Ļ��ּ�˵�����£�
		--    �ɽ�����5���񼣻���
		--    ����ֵ��30���񼣻��֣�ÿ��ֻ�ɶһ�һ�Σ�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	2230)		--�һ��ɽ�������1
		msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	2231)		--�һ��ɽ�������5
		msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	2232)		--�һ�����ֵ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then
		if ShenJiPoint<5 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2233)	--�����񼣻��ֲ��㣬�޷��һ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if FreeSize>=1 then
				role.ModifyMiraclePoints(MapID, InstanceID, RoleID, -5)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3300103, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2234)	--������5���񼣻��ֶһ�һö�ɽ�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2235)	--�����ҿռ䲻�㣬���������ҿռ�������һ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

	elseif TalkIndex == 5 then
		if ShenJiPoint<25 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2233)	--�����񼣻��ֲ��㣬�޷��һ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if FreeSize>=1 then
				role.ModifyMiraclePoints(MapID, InstanceID, RoleID, -25)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3300103, 5, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2236)	--������25���񼣻��ֶһ���ö�ɽ�����
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2235)	--�����ҿռ䲻�㣬���������ҿռ�������һ���
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

	elseif TalkIndex == 6 then
		if ShenJiPoint<30 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2233)	--�����񼣻��ֲ��㣬�޷��һ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local Cur_D = tonumber(os.date("%j"))
			if Cur_D ~= Last_Recv_D then
				local LevelUpNeedExp = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)
				local addexp = LevelUpNeedExp * 150 / (rolelevel+10)^2 * 5
				role.ModifyMiraclePoints(MapID, InstanceID, RoleID, -30)
				role.AddRoleExp(MapID, InstanceID, RoleID, addexp)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Last_Recv_D"], Cur_D)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2237)	--������30���񼣻��ֶһ�XX�㾭��ֵ����ͬ�ȼ��һ���õľ���ֵ��ͬ����
				msg.AddMsgEvent(MsgID, 9, addexp)
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2238)	--����������ȡ������ֵ����������ֵ����ÿ��ֻ����ȡһ�Σ�
				msg.AddMsgEvent(MsgID, 24,	TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

	end
end

-- ע��
aux.RegisterCreatureEvent(4900524, 7, "n_ShenJiJiangLi")
