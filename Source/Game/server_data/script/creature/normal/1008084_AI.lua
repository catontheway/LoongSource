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
end
aux.RegisterCreatureEvent(1008084, 4, "c1008084_OnDie")



