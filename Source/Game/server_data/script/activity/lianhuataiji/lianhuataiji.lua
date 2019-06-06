--�̶������̫��

LianHuaTaiJi_Mon = {} --����̫��ˢ����̫����ID��
LianHuaTaiJi_Mon[1] = {typeID = 1533001}
LianHuaTaiJi_Mon[2] = {typeID = 1533002}
LianHuaTaiJi_Mon[3] = {typeID = 1533003}
LianHuaTaiJi_Mon[4] = {typeID = 1533004}
LianHuaTaiJi_Mon[5] = {typeID = 1533005}
LianHuaTaiJi_Mon[6] = {typeID = 1533006}
LianHuaTaiJi_Mon[7] = {typeID = 1533007}

LianHuaTaiJi_Pos = {}
LianHuaTaiJi_Pos[1] = {x = 1431 ,y = 9260 ,z = 1712}
LianHuaTaiJi_Pos[2] = {x = 1317 ,y = 9422 ,z = 2251}
LianHuaTaiJi_Pos[3] = {x = 2097 ,y = 9271 ,z = 1289}
LianHuaTaiJi_Pos[4] = {x = 1652 ,y = 9696 ,z = 250}
LianHuaTaiJi_Pos[5] = {x = 2683 ,y = 15589 ,z = 464}
LianHuaTaiJi_Pos[6] = {x = 2516 ,y = 7658 ,z = 1317}
LianHuaTaiJi_Pos[7] = {x = 3647 ,y = 12803 ,z = 3239}
LianHuaTaiJi_Pos[8] = {x = 2214 ,y = 12436 ,z = 3368}
LianHuaTaiJi_Pos[9] = {x = 1077 ,y = 7962 ,z = 2693}
LianHuaTaiJi_Pos[10] = {x = 608 ,y = 13363 ,z = 3386}

LianHuaTaiJi_Total = 0
LianDanLu_Max = 10 --ÿ10����ˢ������
LianHuaTaiJi_TimerMin = 0

function LianHuaTaiJi_OnStart(actID)

	LianHuaTaiJi_TimerMin = 0
	LianHuaTaiJi_Total = 0

	--������ȫ��ͨ�棺����������̫������ڿ�ʼ����ͼ����ˢ��10������¯����ҿ�������ʯ��̫��ʯͶ��¯�л�ȡ�������飬��ɱ̫�������ɻ����ϡ��������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 830)
	msg.DispatchMapMsgEvent(MsgID, 3017299407, -1)

	--���س����ڵĲ߻��༭�ƶ��ص�ˢ�³�10��������̬NPC������¯��

	for k=1, LianDanLu_Max do
		local x = LianHuaTaiJi_Pos[k].x
		local y = LianHuaTaiJi_Pos[k].y
		local z = LianHuaTaiJi_Pos[k].z

		--���ɹ���
		local CreatureID = map.MapCreateCreature(3017299407, -1, 3021006, x, y, z)
	end
end

function LianHuaTaiJi_OnTimerMin(actID)

	LianHuaTaiJi_TimerMin = LianHuaTaiJi_TimerMin + 1

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
		if LianHuaTaiJi_TimerMin == broadmin[i] then
			--����������ͨ�棺��1���Ӻ��ٴ�ˢ��10������¯�����λ��ʿ����׼����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 831)
			msg.DispatchMapMsgEvent(MsgID, 3017299407, -1)
		end
	end

	for i=1, 5 do
		if LianHuaTaiJi_TimerMin == respawnmin[i] then
			--��ʼ���ȫ�ֱ���
			--���س����ڵĲ߻��༭�ƶ��ص�ˢ�³�10��������̬NPC������¯��
			for k=1, LianDanLu_Max do
				local x = LianHuaTaiJi_Pos[k].x
				local y = LianHuaTaiJi_Pos[k].y
				local z = LianHuaTaiJi_Pos[k].z
				--���ɹ���
				local CreatureID = map.MapCreateCreature(3017299407, -1, 3021006, x, y, z)
			end
		end
	end
end


--ע��û������¼�
aux.RegisterActEvent(61, 2, "LianHuaTaiJi_OnStart")
aux.RegisterActEvent(62, 2, "LianHuaTaiJi_OnStart")
aux.RegisterActEvent(63, 2, "LianHuaTaiJi_OnStart")
aux.RegisterActEvent(64, 2, "LianHuaTaiJi_OnStart")

aux.RegisterActEvent(61, 4, "LianHuaTaiJi_OnTimerMin")
aux.RegisterActEvent(62, 4, "LianHuaTaiJi_OnTimerMin")
aux.RegisterActEvent(63, 4, "LianHuaTaiJi_OnTimerMin")
aux.RegisterActEvent(64, 4, "LianHuaTaiJi_OnTimerMin")


--������¯�Ի�
function LianDanLu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

		local Num = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1) --��¼Ͷ�����
		local Count = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2) --��¼��ˢ�ֶ�Ӧ�ķ���
		local HaveDeleteStone = 0
		local YinYangCanSummon = 0
	if TalkIndex == -1 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level < 70 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 842) --��δ�ﵽ70�����޷��μӴ˻��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			TalkIndex = -1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 832)	-- "������¯��Ͷ�ŵ��߿��Ի�ô�������ֵ����ҪͶ�����֣�\n��ǰ����¯�ѱ�Ͷ��<p1>��"
			msg.AddMsgEvent(MsgID, 9, Num)	--���뵱ǰ¯�ӵ�Ͷ�����
			msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
			msg.AddMsgEvent(MsgID, 1, 833)	-- "Ͷ������ʯ�������ͨ��������"
			msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
			msg.AddMsgEvent(MsgID, 1, 834)	-- "Ͷ��̫��ʯ����ý϶���������"
			msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ����
			msg.AddMsgEvent(MsgID, 1, 835)	-- "Ͷ��𵤣���ô�������"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			TalkIndex = -1
		end
	elseif TalkIndex == 4 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612801)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612801, 1, 101)
			Num = Num + 1
			Count = Count + 1
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 15000)

			HaveDeleteStone = 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	elseif TalkIndex == 5 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612802)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612802, 1, 101)
			Num = Num + 1
			Count = Count + 2
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 60000)

			HaveDeleteStone = 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	elseif TalkIndex == 6 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612803)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612803, 1, 101)
			Num = Num + 1
			Count = Count + 2
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 74000)

			HaveDeleteStone = 1

		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	end


	if 	HaveDeleteStone == 1 then
		if Num == 5 then
			local Index = 0
			if Count == 10 then
				Index = 7
			elseif Count >= 7 then
				Index = math.random(4,6)
			elseif Count > 0 then
				Index = math.random(1,3)
			end
			local MonsterID = LianHuaTaiJi_Mon[Index].typeID
			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, MonsterID, x, y, z)
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			LianHuaTaiJi_Total = LianHuaTaiJi_Total + 1
			YinYangCanSummon = 1
		else
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1, Num)
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, Count)
		end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 840)	-- "��ǰ����¯�ѱ�Ͷ��<p1>��"
			msg.AddMsgEvent(MsgID, 9, Num)	--���뵱ǰ¯�ӵ�Ͷ�����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			--���ӽ���ʵ��ֵ
			Increase_shili(MapID, InstanceID, RoleID, 0, 5,"jinjieshili_A")
	end

	if LianHuaTaiJi_Total == 40 and YinYangCanSummon == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 836)--"���ڱ�������̫��������ﵽ40���������س��г��־޴������̫�������ɻ�ñ�����¯����ľ��顣"
		msg.DispatchMapMsgEvent(MsgID, 3017299407, -1)
		map.MapCreateCreature(MapID, InstanceID, 3021007,1845, 14024, 1974)
	end
end

aux.RegisterCreatureEvent(3021006, 7, "LianDanLu_OnTalk")


--����̫�����Ի��¼�
function YinYang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local HaveDeleteStone = 0
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["YinYangNum"])
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20240)
	if TalkIndex == -1 then
		if bhave == true then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 837)	-- "���Ѿ�Ͷ����3���ˣ���ȴ������Ǿ����֡�"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			TalkIndex = -1
		else
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level < 70 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 842) --��δ�ﵽ70�����޷��μӴ˻��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 838)	-- "������̫������Ͷ�ŵ��߿��Ի�ô�������ֵ����ҪͶ�����֣���ע��ÿ�����ֻ������Ͷ3�ε��ߡ�"
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 833)	-- "Ͷ������ʯ�������ͨ��������
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 834)	-- "Ͷ��̫��ʯ����ý϶���������"
				msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ����
				msg.AddMsgEvent(MsgID, 1, 835)	-- "Ͷ��𵤣���ô�������"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			end
		end
	elseif TalkIndex == 4 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612801)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612801, 1, 101)
			Num = Num + 1
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 15000*2)

			HaveDeleteStone = 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	elseif TalkIndex == 5 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612802)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612802, 1, 101)
			Num = Num + 1
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 60000*2)

			HaveDeleteStone = 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	elseif TalkIndex == 6 then
		local StoneNum = role.GetRoleItemNum(RoleID, 2612803)
		if StoneNum > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612803, 1, 101)
			Num = Num + 1
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, 54000*2)
			HaveDeleteStone = 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 841) --��������û�ж�Ӧ��ʯͷ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		TalkIndex = -1
	end

	if 	HaveDeleteStone == 1 then
		if Num >= 3 then
			unit.AddBuff(MapID, InstanceID, RoleID, 2024001, RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["YinYangNum"],0)
		else
			role.SetRoleScriptData(RoleID, 1, RoleDataType["YinYangNum"],Num)
		end
	end
end

aux.RegisterCreatureEvent(3021007, 7, "YinYang_OnTalk")


function C3021007_OnDisappear(MapID, InstanceID, CreatureID, CreatureTypeID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 839)--"����̫��������ʱ���ѵ��������Ǿ��ѳ�������Ҷ���⣬���·Ӣ��ǰ����ս��"
	msg.DispatchWorldMsgEvent(MsgID)
	map.MapCreateCreature(MapID, InstanceID, 1533008, 1845, 14024, 1974)

end

aux.RegisterCreatureEvent(3021007, 13, "C3021007_OnDisappear")


function taijiding_Dead(MapID, InstanceID, CreatureID, TypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 10,"jinjieshili_A")
end

function C1533008_Dead(MapID, InstanceID, CreatureID, TypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 20,"jinjieshili_A")
end
aux.RegisterCreatureEvent(1533001, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533002, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533003, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533004, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533005, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533006, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533007, 4, "taijiding_Dead")
aux.RegisterCreatureEvent(1533008, 4, "C1533008_Dead")


