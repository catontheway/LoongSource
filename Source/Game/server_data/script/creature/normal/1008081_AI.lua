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
end
aux.RegisterCreatureEvent(1008081, 4, "c1008081_OnDie")




