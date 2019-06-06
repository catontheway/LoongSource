-- �������ս������ʾ��
function c1008089_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ5�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008089, 2, "c1008089_OnEnterCombat")

--��������
function c1008089_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")
	--��һ�ɱ��������ħ��[1008089]���Ź㲥
	local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���
	local r = math.random(1,100);	--ȡ�����

	msg.AddMsgEvent(MsgID,102,100024);
	msg.AddMsgEvent(MsgID,2,RoleID);
	if ( r<11 ) then
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
	elseif ( r<61 ) then
		msg.DispatchBroadcast(MsgID,3017299151,-1,-1);--����
		msg.DispatchBroadcast(MsgID,3308687566,-1,-1);--������
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	else
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	end
end
aux.RegisterCreatureEvent(1008089, 4, "c1008089_OnDie")

-- �������ս������
function c1008081_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40049)
	end
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008081, 2, "c1008081_OnEnterCombat")


--����ɱ ������ɷ��ħ�� 1008081 ˢ�� ������ɷ��ħ�� 1008082 ���꣺x 2840 y 26387 z 354
function c1008081_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008082, 2840, 26387, 354)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1770)	--������ɷ��ħ�������³���������[2840, 354]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008081, 4, "c1008081_OnDie")

-- �������ս������
function c1008082_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40050)
	end
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008082, 2, "c1008082_OnEnterCombat")


--����ɱ ������ɷ��ħ�� 1008082 ˢ�� ������ɷ��ħ�� 1008083 ���꣺x 2791 y 26387 z 734
function c1008082_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008083, 2791, 26387, 734)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1771)	--������ɷ��ħ�������³���������[2791, 734]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008082, 4, "c1008082_OnDie")

-- �������ս������
function c1008083_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40051)
	end
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008083, 2, "c1008083_OnEnterCombat")


--����ɱ ������ɷ��ħ�� 1008083 ˢ�� ������ɷ��ħ�� 1008084 ���꣺x 2379 y 26387 z 737
function c1008083_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008084, 2379, 26387, 737)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1772)	--������ɷ��ħ�������³���������[2379, 737]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008083, 4, "c1008083_OnDie")

-- �������ս������
function c1008084_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40052)
	end
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008084, 2, "c1008084_OnEnterCombat")


--����ɱ ������ɷ��ħ�� 1008084 ˢ�� ������ɷ��ħ�� 1008085 ���꣺x 2820 y 26387 z 302
function c1008084_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008085, 2820, 26387, 302)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1773)	--������ɷ��ħ�������³���������[2820, 302]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008084, 4, "c1008084_OnDie")

-- �������ս������
function c1008085_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40053)
	end
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1008085, 2, "c1008085_OnEnterCombat")

--����ɱ ������ɷ��ħ�� 1008085 ˢ�� ����������ħ 1008089 ���꣺x 2600 y 25797 z 447
function c1008085_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008089, 2600, 25797, 447)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1774)	--����������ħ�ѳ���������[2600, 447]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008085, 4, "c1008085_OnDie")
