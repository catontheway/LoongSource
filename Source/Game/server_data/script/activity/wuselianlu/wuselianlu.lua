--�̶����ɫ��¯

Wu_Se_Lian_Lu_Produce = {}
Wu_Se_Lian_Lu_Num = {}

Wu_Se_Lian_Lu_Total = 0

act11_TimerMin = 0

function act11_OnStart(actID)
	--��ʼ���ȫ�ֱ���
	for k=1, 30 do
		table.remove(Wu_Se_Lian_Lu_Produce)
		table.remove(Wu_Se_Lian_Lu_Num)
	end

	act11_TimerMin = 0
	Wu_Se_Lian_Lu_Total = 0

	--������ȫ��ͨ�棺��������ɫ��¯����ڿ�ʼ����ҿ�ӻԾ�μӡ�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 35)
	msg.DispatchMapMsgEvent(MsgID, 3017298383, -1)

	--�ڷ��賡���ڵĲ߻��༭�ƶ��ص�ˢ�³�30��������̬NPC����ɫ��¯��
	for k=1, 30 do
		local x = map_list[3017298383].map[k].x
		local y = map_list[3017298383].map[k].y
		local z = map_list[3017298383].map[k].z

		--���ɹ���
		local CreatureID = map.MapCreateCreature(3017298383, -1, 3020518, x, y, z)
		table.insert(Wu_Se_Lian_Lu_Produce, CreatureID, 0)
		table.insert(Wu_Se_Lian_Lu_Num, CreatureID, 0)
	end
end

function act11_OnTimerMin(actID)

	act11_TimerMin = act11_TimerMin+1

	local broadmin={}
	broadmin[1] = 9
	broadmin[2] = 19
	broadmin[3] = 29
	broadmin[4] = 39
	broadmin[5] = 49

	local respawnmin={}
	respawnmin[1] = 10
	respawnmin[2] = 20
	respawnmin[3] = 30
	respawnmin[4] = 40
	respawnmin[5] = 50

	for i=1, 5 do
		if act11_TimerMin == broadmin[i] then
			--������ȫ��ͨ�棺������������1����������ˢ����ɫ��¯��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 36)
			msg.DispatchMapMsgEvent(MsgID, 3017298383, -1)
		end
	end

	for i=1, 5 do
		if act11_TimerMin == respawnmin[i] then


			--��ʼ���ȫ�ֱ���
			for k=1, 30 do
				table.remove(Wu_Se_Lian_Lu_Produce)
				table.remove(Wu_Se_Lian_Lu_Num)
			end
			--�ڷ��賡���ڵĲ߻��༭�ƶ��ص�ˢ�³�10��������̬NPC����ɫ��¯��
			for k=1, 30 do
				local x = map_list[3017298383].map[k].x
				local y = map_list[3017298383].map[k].y
				local z = map_list[3017298383].map[k].z

				--���ɹ���
				local CreatureID = map.MapCreateCreature(3017298383, -1, 3020518, x, y, z)
				table.insert(Wu_Se_Lian_Lu_Produce, CreatureID, 0)
				table.insert(Wu_Se_Lian_Lu_Num, CreatureID, 0)
			end
		end
	end
end


--ע��û������¼�
aux.RegisterActEvent(52, 2, "act11_OnStart")
aux.RegisterActEvent(53, 2, "act11_OnStart")
aux.RegisterActEvent(54, 2, "act11_OnStart")
aux.RegisterActEvent(55, 2, "act11_OnStart")

aux.RegisterActEvent(52, 4, "act11_OnTimerMin")
aux.RegisterActEvent(53, 4, "act11_OnTimerMin")
aux.RegisterActEvent(54, 4, "act11_OnTimerMin")
aux.RegisterActEvent(55, 4, "act11_OnTimerMin")

--��ɫʯ��ֵ
Wu_Se_Shi = {}
Wu_Se_Shi[1352003] = {Value = 5}
Wu_Se_Shi[2611630] = {Value = 5}
Wu_Se_Shi[2611631] = {Value = 4}
Wu_Se_Shi[2611632] = {Value = 3}
Wu_Se_Shi[2611633] = {Value = 2}
Wu_Se_Shi[2611634] = {Value = 1}


--�ɲ���ֵ���߲���¯��Ӧ��
Qi_Cai_Table = {}
Qi_Cai_Table[5] = {TypeID = 1004641}
Qi_Cai_Table[6] = {TypeID = 1004641}
Qi_Cai_Table[7] = {TypeID = 1004641}
Qi_Cai_Table[8] = {TypeID = 1004641}
Qi_Cai_Table[9] = {TypeID = 1004642}
Qi_Cai_Table[10] = {TypeID = 1004642}
Qi_Cai_Table[11] = {TypeID = 1004642}
Qi_Cai_Table[12] = {TypeID = 1004642}
Qi_Cai_Table[13] = {TypeID = 1004643}
Qi_Cai_Table[14] = {TypeID = 1004643}
Qi_Cai_Table[15] = {TypeID = 1004643}
Qi_Cai_Table[16] = {TypeID = 1004644}
Qi_Cai_Table[17] = {TypeID = 1004644}
Qi_Cai_Table[18] = {TypeID = 1004644}
Qi_Cai_Table[19] = {TypeID = 1004645}
Qi_Cai_Table[20] = {TypeID = 1004645}
Qi_Cai_Table[21] = {TypeID = 1004645}
Qi_Cai_Table[22] = {TypeID = 1004646}
Qi_Cai_Table[23] = {TypeID = 1004646}
Qi_Cai_Table[24] = {TypeID = 1004646}
Qi_Cai_Table[25] = {TypeID = 1004647}

--������ɫ��¯
function C3020518_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	--�������Ƿ�����ɫʯ���������ֱ�ӿ۳�

	local rolehavewuseshi = 0

	if cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, Wu_Se_Lian_Lu_Num[CreatureID]) == RoleID then
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 108)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 1
	end

	local wuseshi_id={}
	wuseshi_id[1]=1352003
	wuseshi_id[2]=2611630
	wuseshi_id[3]=2611631
	wuseshi_id[4]=2611632
	wuseshi_id[5]=2611633
	wuseshi_id[6]=2611634

	for i=1, 6 do

		k = wuseshi_id[i]

		local nRet = role.RemoveFromRole(MapID, InstanceID, RoleID, k, 1, 1000)
		if 0 == nRet then
			--ɾ���ɹ����������ɫʯ������ֵ
			Wu_Se_Lian_Lu_Produce[CreatureID] = Wu_Se_Lian_Lu_Produce[CreatureID] + Wu_Se_Shi[k].Value
			Wu_Se_Lian_Lu_Num[CreatureID] = Wu_Se_Lian_Lu_Num[CreatureID] + 1
			rolehavewuseshi = 1

			--���ӽ���ʵ��ֵ
			Increase_shili(MapID, InstanceID, RoleID, 0, 3, "jinjieshili_A")


			if Wu_Se_Lian_Lu_Num[CreatureID]~=5 then
				cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, Wu_Se_Lian_Lu_Num[CreatureID], RoleID)
			end

			local addexp = {}
			addexp[30] = 2850
			addexp[31] = 3050
			addexp[32] = 3250
			addexp[33] = 3450
			addexp[34] = 3650
			addexp[35] = 3850
			addexp[36] = 4050
			addexp[37] = 4250
			addexp[38] = 4450
			addexp[39] = 4650
			addexp[40] = 4850
			addexp[41] = 5050
			addexp[42] = 5250
			addexp[43] = 5450
			addexp[44] = 5650
			addexp[45] = 5850
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

			if level>45 then
				level=45
			end

			if k==1352003 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addexp[level]*1.5)
			else
				role.AddRoleExp(MapID, InstanceID, RoleID, addexp[level])
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 39)
			if k==1352003 then
				msg.AddMsgEvent(MsgID, 9, addexp[level]*1.5)	--����ֵ
			else
				msg.AddMsgEvent(MsgID, 9, addexp[level])
			end
			msg.AddMsgEvent(MsgID, 9, Wu_Se_Lian_Lu_Num[CreatureID])	--��¯����ɫʯ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			--5���ύ��������ܺ�ֵ������ͬ���߲���¯
			if 5 == Wu_Se_Lian_Lu_Num[CreatureID] then
				local Produce = Wu_Se_Lian_Lu_Produce[CreatureID]
				local TypeID = Qi_Cai_Table[Produce].TypeID
				local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)

				--ˢ���߲���¯
				map.MapCreateCreature(MapID, InstanceID, TypeID, x+4, y, z+4)
				Wu_Se_Lian_Lu_Total = Wu_Se_Lian_Lu_Total+1

				if Wu_Se_Lian_Lu_Total<30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 109)
					msg.AddMsgEvent(MsgID, 9, Wu_Se_Lian_Lu_Total)
					msg.DispatchMapMsgEvent(MsgID, 3017298383, -1)
				elseif Wu_Se_Lian_Lu_Total==30 then
					--ˢ����ɫ�߲�¯
					map.MapCreateCreature(MapID, InstanceID, 3020519, 923, 10000, 1940)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 110)
					msg.DispatchMapMsgEvent(MsgID, 3017298383, -1)
				end

			end
			break
		end
	end
	if rolehavewuseshi == 0 then
		unit.ChangeHP(MapID, InstanceID, CreatureID, 1)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 112)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	end
end
--ע�������ɫ��¯�¼�
aux.RegisterCreatureEvent(3020518, 6, "C3020518_OnInVest")


--��ɫ�߲�¯�����¼�
function C3020519_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	--�������Ƿ�����ɫʯ���������ֱ�ӿ۳�

	k = 1352003

	local nRet = role.RemoveFromRole(MapID, InstanceID, RoleID, k, 1, 1000)
	if 0 == nRet then
		--���ӽ���ʵ��ֵ
		local shili = role.GetRoleScriptData(RoleID, 1, RoleDataType["jinjieshili"])
		role.SetRoleScriptData(RoleID, 1, RoleDataType["jinjieshili"],shili+0.25)

		--ɾ���ɹ����������ɫʯ������ֵ
		local addexp = {}
		addexp[30] = 2850
		addexp[31] = 3050
		addexp[32] = 3250
		addexp[33] = 3450
		addexp[34] = 3650
		addexp[35] = 3850
		addexp[36] = 4050
		addexp[37] = 4250
		addexp[38] = 4450
		addexp[39] = 4650
		addexp[40] = 4850
		addexp[41] = 5050
		addexp[42] = 5250
		addexp[43] = 5450
		addexp[44] = 5650
		addexp[45] = 5850
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level>45 then
			level=45
		end
		if k==1352003 then
			role.AddRoleExp(MapID, InstanceID, RoleID, addexp[level]*1.5)
		else
			role.AddRoleExp(MapID, InstanceID, RoleID, addexp[level])
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 111)
		if k==1352003 then
			msg.AddMsgEvent(MsgID, 9, addexp[level]*1.5)	--����ֵ
		else
			msg.AddMsgEvent(MsgID, 9, addexp[level])
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 113)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(3020519, 6, "C3020519_OnInVest")


function C3020519_OnDisappear(MapID, InstanceID, CreatureID, CreatureTypeID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 114)
	msg.DispatchMapMsgEvent(MsgID, 3017298383, -1)

	map.MapCreateCreature(MapID, InstanceID, 1004650, 923, 10000, 1940)

end

aux.RegisterCreatureEvent(3020519, 13, "C3020519_OnDisappear")

--��ɱ������¯��ԭ��ˢ�������ɫ����
function C1004643_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1004648, x, y, z)
end

--��ɱ�װ���¯��ԭ��ˢ������߲�Ԫ��
function C1004647_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1004649, x, y, z)
end

aux.RegisterCreatureEvent(1004643, 4, "C1004643_OnDie")
aux.RegisterCreatureEvent(1004647, 4, "C1004647_OnDie")




function C100464x_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
		if CreatureTypeID == 1004648 then
			Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")
		elseif CreatureTypeID == 1004649 then
			Increase_shili(MapID, InstanceID, RoleID, 1, 6, "jinjieshili")
		end
end

aux.RegisterCreatureEvent(1004648, 4, "C100464x_OnDie")
aux.RegisterCreatureEvent(1004649, 4, "C100464x_OnDie")

















