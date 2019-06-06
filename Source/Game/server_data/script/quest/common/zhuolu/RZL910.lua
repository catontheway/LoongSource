--RZL910
--ʹ�����ľϻ
--�����þ�mapscriptdata(1,0)��¼����BOSS�Ƿ�ɱ���������ɱ���������ˢ��С�֡�
function i3200565_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	local i = role.GetBagFreeSize(TargetID)
	if i <= 2 then
	    bRet = 40
	end
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200565_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.AddRoleItem(MapID, InstanceID, TargetID, 3200571, 1, -1, 8, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 3200532, 1, -1, 8, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 3200533, 1, -1, 8, 420)
end

--ע��
aux.RegisterItemEvent(3200565, 1, "i3200565_QuestUsable")
aux.RegisterItemEvent(3200565, 0, "i3200565_CanUse")




--ʹ����������Х��Ұ
function i3200532_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i=role.GetRoleAttValue(MapID, InstanceID, TargetID, 24)
	if i <= 49 then
	    bRet = 32
	end
	--�������Q02������ʹ��
	if MapID ~= 1826990375 then
	    bRet = 43
	end

	--����
	return bRet, bIgnore
end

--ʹ��Ч��,ʹ��������Ұ������50����
function i3200532_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 4040103, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 2019601, TargetID)
end

--ע��
aux.RegisterItemEvent(3200532, 1, "i3200532_QuestUsable")
aux.RegisterItemEvent(3200532, 0, "i3200532_CanUse")

--ʹ����������������
function i3200533_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	local i=role.GetRoleAttValue(MapID, InstanceID, TargetID, 24)
	if i <= 49 then
	    bRet = 32
	end
	--�������Q02������ʹ��
	if MapID ~= 1826990375 then
	    bRet = 43
	end
	--����
	return bRet, bIgnore
end

--ʹ��Ч��,ʹ�û������Σ�����50����
function i3200533_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 4040403, TargetID)
       unit.AddBuff(MapID, InstanceID, TargetID, 2019601, TargetID)
end

--ע��
aux.RegisterItemEvent(3200533, 1, "i3200533_QuestUsable")
aux.RegisterItemEvent(3200533, 0, "i3200533_CanUse")



--���������ظ�����ֵ
function c1531007_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 2017201, RoleID)
end

aux.RegisterCreatureEvent(1531007, 4, "c1531007_OnDie")

--���������ظ�����ֵ
function c1531008_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.SetMapScriptData(MapID, InstanceID, 1, 0, 0)
end

aux.RegisterCreatureEvent(1531008, 4, "c1531008_OnDie")

--NPC�Ի�����
function x12910_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
    --����Ƿ�Ϊ�����
	if npctypeid == 3011002 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 3200565, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(12910, 6, "x12910_OnNPCTalk")


--�����ȡʱ����
--function x12910_OnAccept(MapID, InstanceID, questID, ownerID, accepterID)
--	role.RoleGotoNewMap(MapID, InstanceID, ownerID, 1826990375, 171, 5, 32)

--end

--ע��
--aux.RegisterQuestEvent(12910, 0, "x12910_OnAccept")


