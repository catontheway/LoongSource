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
end
aux.RegisterCreatureEvent(1008082, 4, "c1008082_OnDie")



