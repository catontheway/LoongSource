

--�����ڻ

--������ʹ�߶Ի�����


jizurenwushuliang = 0

function Chongyangjieshizhe_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4101) -- "������ʹ�ߣ�\n ũ�����¾���Ϊ��ͳ�������ڣ��ֳơ����˽ڡ�����Ϊ���׾����аѡ�������Ϊ�������ѡ��š���Ϊ���������¾��գ����²������������أ��ʶ���������Ҳ���ؾš�����������ս��ʱ�ھ��Ѿ��γɣ������ƴ�����������ʽ��Ϊ���Ľ��գ��˺�����������Ϯ����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4102) -- �������ڻʱ�䡱
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4103) -- "�����ڻ����-���μ���"
		msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 4104)   --"�����ڻ����-�Ǹ�Ѱ��"
		msg.AddMsgEvent(MsgID, 21, 7) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4105) -- "�鿴������ɻ���"
		--msg.AddMsgEvent(MsgID, 21, 8)   --ȡ����ť
		--msg.AddMsgEvent(MsgID, 1, 4106)   --"��-��������δ���棩"
		msg.AddMsgEvent(MsgID, 21, 9)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 4107)   --"��ȡ���񣺽��μ��棨һ��"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

	if TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4108) -- 2010��10��15����2010��10��19��
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4125) -- ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

	if TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4109) -- ÿ����ɫ���������ʼ�Ժ󣬾��������������ȡϵ�����񡰽��μ��桱����ϵ������һ��5������ȡÿ������ʱ�������6�����ÿ��������ɻ�ò�ͬ��������1��1��������ÿ���������1�������Ϲ�Ǯ�ң����ң������ڵǸ�Ѱ�ɻ�жһ����ֽ�����ע�����������������Ļ���������¼��Ӱ��Ǹ�Ѱ�ɵĽ���������������鿴�Ǹ�Ѱ�ɵĻ˵����
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4125) -- ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

	if TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4110) -- �������ʼ�Ժ������ǣ�2184��2355��������·����ˣ������������ʹ���Ϲ�Ǯ�Ҷһ������⣨ʹ�ú����̽��͸����������5%�����������֣��ָ�ʱ��30�룩����10��ļ۸����Ϲ�Ǯ�ҡ����⵱����������ɡ����μ��桱����Ļ����ﵽ100ʱ���ֱ������ǣ�����2184��2377����������2183��2418�������·����ˣ�������߽�������ݶ��ϣ����ɷֱ�ʹ���Ϲ�Ǯ������������һ��ͼ��ػ�ʯ���м��ػ�ʯ���ɸ���������100����ɫ��δ���棩�ȸ�����ϡ���ߣ����в����������������ۣ���ע��ÿ��18:00�������ά��ʱ�����д����ᱻ���ã�֮ǰ���ֵ��·�����Ҳ����֮��ʧ��
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4125) -- ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

	if TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4111) -- ��ǰ�����������������ܻ���ΪX��ÿ��18:00�������ά��ʱ�����ᱻ���ã�
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 12, jizurenwushuliang) -- ��������ܻ���
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4125) -- ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end

	--[[if TalkIndex == 8 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4112) -- �·����˳��۵ķ�-����������δ���棬��������Ժ���Ҫʹ������ʯ���п������װ��ʹ�ã�����ʯ������Ϸ�̳��ڹ���
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 4125) -- ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end]]--

	if TalkIndex == 9 then
		local a = role.GetRoleSilver(MapID, InstanceID,RoleID)
		local b = role.GetBagFreeSize(RoleID)
		if a < 60000 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4113)   --����������㣬��ȡ��������Ҫ����6��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif b < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4114)   --�����ռ䲻�㣬�޷���ȡ������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20331)  == true then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4126)   --���Ѿ���ȡ�˸������޷��ٽ�ȡ
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4115)   --��ȡ���񣺽��μ��棨һ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				role.AddQuest(RoleID,20331) --������񣺽��μ��棨һ��
				role.DecRoleSilver(MapID, InstanceID, RoleID, 60000, 101) -- �۳�6��
				--role.AddRoleItem(MapID, InstanceID, RoleID, 6000201, 1, -1, 8, 420) 	-- ������������
			end
		end
	end
end

aux.RegisterCreatureEvent(3610232, 7, "Chongyangjieshizhe_OnTalk")


--��ɽ��μ�������
--����ID ��20331-20335

function Chongyang_Jizurenwu(MapID, InstanceID, QuestID, RoleID, TargetID)

	local k = jizurenwushuliang
	jizurenwushuliang = k + 1
	if jizurenwushuliang == 1000 then
		for i = 13, 14 do
			if Chongyang_Creature[i].creid == 0 then
				local MsgID = msg.BeginMsgEvent() -- ���繫��
				msg.AddMsgEvent(MsgID, 100, 4116) -- �·����ˡ��� �� �·����ˡ��� �Ѿ��ֱ���������� ��2184,2377�� �� ��2183,2418��, ���λ������ٸ�ȥ������
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchWorldMsgEvent(MsgID)
				Chongyang_Creature[i].creid =  map.MapCreateColCreature(3017298127, -1, Chongyang_Creature[i].typeid, Chongyang_Creature[i].x, Chongyang_Creature[i].y, Chongyang_Creature[i].z, 1)
			end
		end
	end

end

aux.RegisterQuestEvent(20331, 1, "Chongyang_Jizurenwu")
aux.RegisterQuestEvent(20332, 1, "Chongyang_Jizurenwu")
aux.RegisterQuestEvent(20333, 1, "Chongyang_Jizurenwu")
aux.RegisterQuestEvent(20334, 1, "Chongyang_Jizurenwu")
aux.RegisterQuestEvent(20335, 1, "Chongyang_Jizurenwu")



-- ʹ�����������

--������	�ڡ�2389��3413������¯����ʹ��	6000201
--������	�ڡ�1272��2794������¯����ʹ��	6000202
--������	�ڡ�847��1950������¯����ʹ��	6000203
--������	�ڡ�940��407������¯����ʹ��	6000204
--������	�ڡ�2465��1051������¯����ʹ��	6000205
--������ҽ�	��������	6000207
--������ҽ�	��������	6000208
--������ҽ�	��������	6000209
--������ҽ�	��������	6000210
--������ҽ�	��������	6000211
--���滤�� 3302701


function I6000201_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif (x <= 2379 or x >= 2399) and  (z <= 3403 or z >=3423) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4117)  -- �������� [2389,3413]����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end

function I6000201_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local exp = Level*50
	--unit.AddBuff(MapID, InstanceID, RoleID, 3322701 ,RoleID) -- ����BUFF
	role.AddRoleItem(MapID, InstanceID, RoleID, 6000207, 1, -1, 8, 420) -- ������Իҽ�
	role.AddRoleExp(MapID, InstanceID, RoleID, exp) -- ���Ӿ���
end


function I6000202_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	local x , y , z = unit.GetPosition(MapID, InstanceID,RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif (x <= 1262 or x >= 1282) and  (z <= 2784 or z >=2804) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4118)  -- �������� [1272,2794]����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end

function I6000202_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local exp = Level*50
	--unit.AddBuff(MapID, InstanceID, TargetID, 3322701 ,RoleID) -- ����BUFF
	role.AddRoleItem(MapID, InstanceID, RoleID, 6000208, 1, -1, 8, 420) -- ������Իҽ�
	role.AddRoleExp(MapID, InstanceID, RoleID, exp) -- ���Ӿ���
end


function I6000203_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	local x , y , z = unit.GetPosition(MapID, InstanceID,RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif (x <= 837 or x >= 857) and  (z <= 1940 or z >=1960) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4119)  -- �������� [847,1950]����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end

function I6000203_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local exp = Level*50
	--unit.AddBuff(MapID, InstanceID, RoleID, 3322701 ,RoleID) -- ����BUFF
	role.AddRoleItem(MapID, InstanceID, RoleID, 6000209, 1, -1, 8, 420) -- ������Իҽ�
	role.AddRoleExp(MapID, InstanceID, RoleID, exp) -- ���Ӿ���
end

function I6000204_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	local x , y , z = unit.GetPosition(MapID, InstanceID,RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif (x <= 930 or x >= 950) and  (z <= 397 or z >=417) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4120)  -- �������� [940,407]����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end

function I6000204_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local exp = Level*50
	--unit.AddBuff(MapID, InstanceID, RoleID, 3322701 ,RoleID) -- ����BUFF
	role.AddRoleItem(MapID, InstanceID, RoleID, 6000210, 1, -1, 8, 420) -- ������Իҽ�
	role.AddRoleExp(MapID, InstanceID, RoleID, exp) -- ���Ӿ���
end



function I6000205_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	local x , y , z = unit.GetPosition(MapID, InstanceID,RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif (x <= 2455 or x >= 2475) and  (z <= 1041 or z >= 1061) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4121)  -- �������� [2465,1051]����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end

function I6000205_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local exp = Level*50
	--unit.AddBuff(MapID, InstanceID, RoleID, 3322701 ,RoleID) -- ����BUFF
	role.AddRoleItem(MapID, InstanceID, RoleID, 6000211, 1, -1, 8, 420) -- ������Իҽ�
	role.AddRoleExp(MapID, InstanceID, RoleID, exp) -- ���Ӿ���
end


aux.RegisterItemEvent(6000201, 0, "I6000201_CanUse")
aux.RegisterItemEvent(6000202, 0, "I6000202_CanUse")
aux.RegisterItemEvent(6000203, 0, "I6000203_CanUse")
aux.RegisterItemEvent(6000204, 0, "I6000204_CanUse")
aux.RegisterItemEvent(6000205, 0, "I6000205_CanUse")
aux.RegisterItemEvent(6000201, 1, "I6000201_OnUse")
aux.RegisterItemEvent(6000202, 1, "I6000202_OnUse")
aux.RegisterItemEvent(6000203, 1, "I6000203_OnUse")
aux.RegisterItemEvent(6000204, 1, "I6000204_OnUse")
aux.RegisterItemEvent(6000205, 1, "I6000205_OnUse")


--���������
--[[
6000100	��â��ħ��-��
6000101	����������-��
6000102	��������-��
6000103	����Ǭ����-��
6000104	�һ�����ӡ-��
6000105	����������-��
6000106	����������-��
6000107	����������-��
--]]

function I6000212_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	else
		return 0, bIgnore
	end
end


function I6000212_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)

	local k = math.random(100)
	if k <= 1 and k <= 5 then  --5%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000100) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000100, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=6 and k <= 10 then  --5%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000101) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000101, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=11 and k <= 30 then  --20%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000102) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000102, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=31 and k <= 35 then  --5%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000103) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000103, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=36 and k <= 55 then  --20%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000104) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000104, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=56 and k <= 75 then  --20%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000105) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000105, 1, -1, 8, 420) -- ��÷�����
	elseif
		k >=76 and k <= 80 then  -- 5%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000106) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000106, 1, -1, 8, 420) -- ��÷�����
	else	--20%
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 6000212) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 6000107) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000107, 1, -1, 8, 420) -- ��÷�����
	end
end


--<Msg id="4127" name="chongyangjie_27" desc="��ϲ���&lt;p1&gt;��&lt;p2&gt;�������&lt;p3&gt;"/>

function I3303031_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
		local MsgID = msg.BeginMsgEvent()   -- ���繫��
		msg.AddMsgEvent(MsgID, 100, 4127)--������Ϣ����ʾ��һ�õ���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 4, 3303031) --��õ�������
		msg.AddMsgEvent(MsgID, 4, 3303031) --��õ�������
		msg.DispatchWorldMsgEvent(MsgID)
end

aux.RegisterItemEvent(6000212, 0, "I6000212_CanUse")
aux.RegisterItemEvent(6000212, 1, "I6000212_OnUse")
aux.RegisterItemEvent(3303031, 1, "I3303031_OnUse")    --3303031   �޸�1��ʢ�����ӡ�































