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
end
aux.RegisterCreatureEvent(1008083, 4, "c1008083_OnDie")


