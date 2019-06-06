--С��Ů����һع齱����ȡ�ˣ��ű�

function XiaoLongNv(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local AwayTime = role.GetPlayerAccountOfflineTime(RoleID)
	local CanReceive = role.CanRecAccountRegressionReward(RoleID)
	local FreeSize = role.GetBagFreeSize(RoleID)

	if TalkIndex == -1 then

		if CanReceive ~= true then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2201)
				--��ӭ���ѻع飬��Ʒ������
				--������һع齱���������������ҳ���7��δ��¼��Ϸ������ع�
				--��������������ʱ�������Լ��ɻ�ûع���Ʒ��ͬʱ��ѡ�������κ�һ
				--���������ͳ��ع�������ѿɻ�õ���Ʒ���ݸ��ݻع���ҵĵȼ�������
				--��1-50����һع飺��������1
				--��50-80����һع飺��������1���������ϻ����
				--��80-100����һع飺��������1���������ϻ���ӣ����ʯ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if AwayTime < 259200 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2201)		--����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif AwayTime >= 259200 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2202)		--��СŮ�ӱ����˳ƽ��������С��Ů�ˡ����Ÿ����˽���ǰ����������ȴ���Ӳ�����
				--������Ľ�Ѿã��������Ŵ����س��������ر��º���һ�ݣ�������ʿ��Ҫ�ƴ�Ŷ��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	2203)		--����ȡ�س�����������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

	elseif TalkIndex == 4 then

		if CanReceive == true and FreeSize>=1 then
			if AwayTime >= 259200 and AwayTime<604800 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 1330635, 1, -1, 8, 420)
				role.SetRecAccountRegressionRewardFlag(RoleID, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2204)		--����Ʒ�Ѿ��ŵ���ʿ�������ˣ�ʹ�ú󼴿ɻ�ȡ��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif AwayTime >= 604800 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 1330634, 1, -1, 8, 420)
				role.SetRecAccountRegressionRewardFlag(RoleID, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2204)		--����Ʒ�Ѿ��ŵ���ʿ�������ˣ�ʹ�ú���������������������ĺ��Ѷ��ɻ����Ӧ��������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				local LevelUpNeedExp = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)
				local temp = rolelevel
				if temp<=80 then temp=80 end

				role.AddRoleExp(MapID, InstanceID, RoleID, LevelUpNeedExp*125/math.pow(temp-70, 2))

			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2205)		--��������������������������ȡ���س�������Ʒ����Ϊ��������һ��ո�������ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

-- ע��
aux.RegisterCreatureEvent(3610201, 7, "XiaoLongNv")
