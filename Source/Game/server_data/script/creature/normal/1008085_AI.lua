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
end
aux.RegisterCreatureEvent(1008085, 4, "c1008085_OnDie")



